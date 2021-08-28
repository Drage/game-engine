
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
    width = other.GetWidth();
    height = other.GetHeight();
    bpp = other.GetBPP();
    imageSize = other.GetSize();
    
    if (pixels)
        delete pixels;
    pixels = new unsigned char[imageSize];
    memcpy(pixels, other.GetPixels(), imageSize);
}

ImagePCX::ImagePCX(const std::string &filename)
{
    Load(filename);
}

bool ImagePCX::Load(const std::string &filename)
{
    name = GetFileName(filename);
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
    memcpy(&header, data, sizeof(Header));
    
    // Image dimensions
    width = header.xMax - header.xMin + 1;
    height = header.yMax - header.yMin + 1;
    
    // Bits per pixel
    bpp = 24;
    
    // Check encoding is supported
    if (header.version != 5)
        return false;
    
    // Get image size
    imageSize = (width * height * (bpp/8));
    return true;
}

bool ImagePCX::LoadData(unsigned char *data)
{
    // Allocate memory
    pixels = new unsigned char[imageSize];
    unsigned char *buffer = new unsigned char[width * height];
    unsigned char *palette = new unsigned char[PALETTE_SIZE];
    if (pixels == NULL || buffer == NULL || palette == NULL)
        return false;
    
    // Decode RLE compression
    unsigned char *ptrData = data;
    unsigned bufferPosition = 0;
    while (bufferPosition < (width * height))
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
        palette[i] = data[fileSize - sizeof(Header) - PALETTE_SIZE + i];
    
    // Apply palette
    for (unsigned y = 0; y < height; y++)
    {
        for (unsigned x = 0; x < width; x++)
        {
            pixels[3 * (y * width + x) + 0] = palette[3 * buffer[y * width + x] + 0];
            pixels[3 * (y * width + x) + 1] = palette[3 * buffer[y * width + x] + 1];
            pixels[3 * (y * width + x) + 2] = palette[3 * buffer[y * width + x] + 2];
        }
    }
    
    delete[] buffer;
    delete[] palette;
    return true;
}
