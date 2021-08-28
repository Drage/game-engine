
#include <fstream>
#include "ImagePCX.h"
#include "Debug.h"
#include "MathUtils.h"
#include "FileSystem.h"

using namespace DrageEngine;

ImagePCX::ImagePCX()
{
    
}

ImagePCX::ImagePCX(const Image &other)
{
    m_width = other.GetWidth();
    m_height = other.GetHeight();
    m_bpp = other.GetBPP();
    m_imageSize = other.GetSize();
    
    if (m_pixels)
        delete m_pixels;
    m_pixels = new unsigned char[m_imageSize];
    memcpy(m_pixels, other.GetPixels(), m_imageSize);
}

ImagePCX::ImagePCX(const std::string &filename)
{
    Load(filename);
}

bool ImagePCX::Load(const std::string &filename)
{
    m_name = GetFileName(filename);
    unsigned char* fileData = NULL;
    std::ifstream file;
    
    try
    {
        file.open(filename, std::ios::binary);
        
        if (!file.is_open())
        {
            ERROR("Failed to load image - Could not open file: " + filename);
            throw std::exception();
        }
        
        // Get file size
        file.seekg(0, std::ios_base::end);
        m_fileSize = (long)file.tellg();
        file.seekg(0, std::ios_base::beg);
        
        // Allocate memory
        fileData = new unsigned char[m_fileSize];
        
        if (fileData == NULL)
        {
            ERROR("Failed to load image - Insufficient memory: " + filename);
            throw std::exception();
        }
        
        // Read the file into memory
        file.read((char*)fileData, m_fileSize);
        file.close();
        
        // Read header data
        if (!ReadHeader(fileData))
        {
            ERROR("Failed to load image - Encoding not supported: " + filename);
            throw std::exception();
        }
        
        if (!LoadData(fileData + sizeof(Header)))
        {
            ERROR("Failed to load image - Insufficient memory: " + filename);
            throw std::exception();
        }
        
        // Free temporary storage
        delete[] fileData;
        file.close();
        return true;
    }
    catch (std::exception&)
    {
        if (file.is_open())
            file.close();
        
        if (fileData)
            delete[] fileData;
        
        return false;
    }
}

bool ImagePCX::ReadHeader(unsigned char *data)
{
    // Read header
    memcpy(&m_header, data, sizeof(Header));
    
    // Image dimensions
    m_width = m_header.xMax - m_header.xMin + 1;
    m_height = m_header.yMax - m_header.yMin + 1;
    
    // Bits per pixel
    m_bpp = 24;
    
    // Check encoding is supported
    if (m_header.version != 5)
        return false;
    
    // Get image size
    m_imageSize = (m_width * m_height * (m_bpp/8));
    return true;
}

bool ImagePCX::LoadData(unsigned char *data)
{
    // Allocate memory
    m_pixels = new unsigned char[m_imageSize];
    unsigned char *buffer = new unsigned char[m_width * m_height];
    unsigned char *palette = new unsigned char[PALETTE_SIZE];
    if (m_pixels == NULL || buffer == NULL || palette == NULL)
        return false;
    
    // Decode RLE compression
    unsigned char *ptrData = data;
    unsigned bufferPosition = 0;
    while (bufferPosition < (m_width * m_height))
    {
        if (*ptrData > 0xbf)
        {
            int repeat = *ptrData++ & 0x3f;
            for (int i = 0; i < repeat; i++)
            {
                buffer[bufferPosition++] = *ptrData;
            }
        }
        else
        {
            buffer[bufferPosition++] = *ptrData;
        }
        ptrData++;
    }
    
    // Read palette
    for (int i = 0; i < PALETTE_SIZE; i++)
        palette[i] = data[m_fileSize - sizeof(Header) - PALETTE_SIZE + i];
    
    // Apply palette
    for (unsigned y = 0; y < m_height; y++)
    {
        for (unsigned x = 0; x < m_width; x++)
        {
            m_pixels[3 * (y * m_width + x) + 0] = palette[3 * buffer[y * m_width + x] + 0];
            m_pixels[3 * (y * m_width + x) + 1] = palette[3 * buffer[y * m_width + x] + 1];
            m_pixels[3 * (y * m_width + x) + 2] = palette[3 * buffer[y * m_width + x] + 2];
        }
    }
    
    delete[] buffer;
    delete[] palette;
    return true;
}
