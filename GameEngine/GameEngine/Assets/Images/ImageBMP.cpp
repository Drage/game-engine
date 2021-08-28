
#include <fstream>
#include "ImageBMP.h"
#include "Debug.h"
#include "FileSystem.h"

using namespace DrageEngine;

ImageBMP::ImageBMP()
{
    
}

ImageBMP::ImageBMP(const Image &other)
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

ImageBMP::ImageBMP(const std::string &filename)
{
    Load(filename);
}

bool ImageBMP::Load(const std::string &filename)
{
    m_name = GetFileName(filename);
    unsigned long fileSize;
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
        fileSize = (long)file.tellg();
        file.seekg(0, std::ios_base::beg);
        
        // Allocate memory
        fileData = new unsigned char[fileSize];
        
        if (fileData == NULL)
        {
            ERROR("Failed to load image - Insufficient memory: " + filename);
            throw std::exception();
        }
        
        // Read the file into memory
        file.read((char*)fileData, fileSize);
        file.close();
        
        // Read header data
        if (!ReadHeader(fileData))
        {
            ERROR("Failed to load image - Encoding not supported: " + filename);
            throw std::exception();
        }
        
        if (!LoadData(&fileData[m_pixelDataOffset]))
        {
            ERROR("Failed to load image - Insufficient memory: " + filename);
            throw std::exception();
        }
        
        // Swap colour channels
        BGRtoRGB();
        
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

bool ImageBMP::ReadHeader(unsigned char *data)
{
    // Check file signature
    if (memcmp(data, "BM", 2) != 0)
        return false;
    
    // Offset from start of file to pixel data
    memcpy(&m_pixelDataOffset, &data[10], 4);
    
    // Image dimensions
    memcpy(&m_width, &data[18], 4);
    memcpy(&m_height, &data[22], 4);
    
    // Bits per pixel
    memcpy(&m_bpp, &data[28], 2);
    if (m_bpp != 24)
        return false;
    
    // Compression method
    unsigned compression;
    memcpy(&compression, &data[30], 4);
    if (compression != 0)
        return false;
    
    // Get image size
    m_imageSize = (m_width * m_height * (m_bpp/8));
    return true;
}

bool ImageBMP::LoadData(unsigned char *data)
{
    // Allocate memory for pixel data
    m_pixels = new unsigned char[m_imageSize];
    if (m_pixels == NULL)
        return false;
    
    // Copy pixel data
    memcpy(m_pixels, data, m_width * m_height * (m_bpp/8));
    return true;
}

void ImageBMP::BGRtoRGB()
{
    unsigned long numPixels;
    unsigned char *cur;
    unsigned char temp;
    short pixelSize;
    
    cur = m_pixels;
    numPixels = m_width * m_height;
    
    // Get pixel size in bytes
    pixelSize = m_bpp/8;
    
    for (int i = 0; i != numPixels; i++)
    {
        temp = *cur;        // Get blue value
        *cur = *(cur + 2);  // Swap red value into first position
        *(cur + 2) = temp;  // Put blue value in last position
        
        cur += pixelSize;
    }
}

bool ImageBMP::Save(const std::string &filePath)
{
    // Cant save if we haven't got any data
    if (!m_pixels)
    {
        ERROR("Failed to save image - No pixel data: " + filePath);
        return false;
    }
    
    // Append extension if not present
    std::string outputPath;
    if (filePath.find(".bmp") == std::string::npos)
        outputPath = filePath + ".bmp";
    else
        outputPath = filePath;
    
    // Open file to write to
    std::ofstream file;
    file.open(outputPath, std::ios::out | std::ios::binary);
    if (!file.is_open())
    {
        ERROR("Failed to save image - Could not open write directory: " + filePath);
        return false;
    }
    
    // Write header
    Header header;
    header.signature[0] = 'B';
    header.signature[1] = 'M';
    EndianSwap((unsigned int)m_imageSize + 50, header.fileSize);
    EndianSwap((unsigned int)0, header.padding);
    EndianSwap((unsigned int)50, header.imageOffset);
    EndianSwap((unsigned int)40, header.bitmapInfoHeaderSize);
    EndianSwap(m_width, header.width);
    EndianSwap(m_height, header.height);
    EndianSwap((unsigned short)1, header.numPlanes);
    EndianSwap((unsigned short)m_bpp, header.pixelDepth);
    EndianSwap((unsigned int)0, header.compressionType);
    EndianSwap((unsigned int)m_imageSize, header.imageSize);
    EndianSwap((unsigned int)2800, header.horizResolution);
    EndianSwap((unsigned int)2800, header.vertResolution);
    EndianSwap((unsigned int)0, header.importantColours);
    file.write((char*)&header, sizeof(Header));
    
    // Write pixels
    file.write((char*)m_pixels, m_imageSize);
    
    file.close();
    return true;
}
