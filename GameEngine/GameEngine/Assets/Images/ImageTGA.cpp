
#include <fstream>
#include "ImageTGA.h"
#include "Debug.h"
#include "FileSystem.h"

using namespace DrageEngine;

ImageTGA::ImageTGA()
{
    encoding = 0;
}

ImageTGA::ImageTGA(const std::string &filename)
{
    encoding = 0;
    Load(filename);
}

ImageTGA::ImageTGA(const Image &other)
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

bool ImageTGA::Load(const std::string &filename)
{
    name = GetFileName(filename);
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
        
        if (encoding == RAW_RGB_ENCODING)
            LoadRawData(fileData);
        else if (encoding == RLE_RGB_ENCODING)
            LoadRLEData(fileData);
        
        // Swap order of colour bytes
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

bool ImageTGA::ReadHeader(unsigned char* data)
{
    encoding = data[2];
    if (encoding != RAW_RGB_ENCODING && encoding != RLE_RGB_ENCODING)
        return false;
    
    // Get image width & height
    short x1, y1, x2, y2;
    memcpy(&x1, &data[8], 2);
    memcpy(&y1, &data[10], 2);
    memcpy(&x2, &data[12], 2);
    memcpy(&y2, &data[14], 2);
    width = (x2 - x1);
    height = (y2 - y1);
    
    // Bits per Pixel
    bpp = (unsigned int) data[16];
    
    // Allocate memory for pixel data
    imageSize = (width * height * (bpp/8));
    pixels = new unsigned char[imageSize];
    return pixels != NULL;
}

void ImageTGA::LoadRawData(unsigned char* data)
{
    // Set offset to start of image data (after header)
    short offset = data[0] + 18;
    
    memcpy(pixels, &data[offset], imageSize);
}

void ImageTGA::LoadRLEData(unsigned char* data)
{
    short offset, pixelSize;
    unsigned char *cur;
    unsigned long index = 0;
    unsigned char numPixels;
    
    // Set offset to start of image data (after header)
    offset = data[0] + 18;
    
    // Get pixel size in bytes
    pixelSize = bpp/8;
    
    // Set pointer to the beginning of the image data
    cur = &data[offset];
    
    // Decode RLE
    while (index < imageSize)
    {
        if (*cur & 0x80) // 10000000
        {
            // Get number of repeated pixels
            numPixels = *cur - 127;
            cur++;
            
            // Repeat the next pixel numPixels times
            for (int i = 0; i != numPixels; i++, index += pixelSize)
                memcpy(&pixels[index], cur, pixelSize);
            
            cur += pixelSize;
        }
        else // Raw data segment
        {
            // Get number of raw pixels
            numPixels = *cur + 1;
            cur++;
            
            for (int i = 0; i != numPixels; i++, index += pixelSize, cur += pixelSize)
                memcpy(&pixels[index], cur, pixelSize);
        }
    }
}

void ImageTGA::BGRtoRGB()
{
    unsigned long numPixels;
    unsigned char *cur;
    unsigned char temp;
    short pixelSize;
    
    cur = pixels;
    numPixels = width * height;
    
    // Get pixel size in bytes
    pixelSize = bpp/8;
    
    for (int i = 0; i != numPixels; i++)
    {
        temp = *cur;        // Get blue value
        *cur = *(cur + 2);  // Swap red value into first position
        *(cur + 2) = temp;  // Put blue value in last position
        
        cur += pixelSize;
    }
}

bool ImageTGA::Save(const std::string &filePath)
{
    // Cant save if we haven't got any data
    if (!pixels)
    {
        ERROR("Failed to save image - No pixel data: " + filePath);
        return false;
    }
    
    // Append extension if not present
    std::string outputPath;
    if (filePath.find(".tga") == std::string::npos)
        outputPath = filePath + ".tga";
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
    header.idLength = 0;
    header.colourMapType = 0;
    header.imageType = RAW_RGB_ENCODING;
    for (int i = 0; i < 5; i++)
        header.colourMap[i] = 0;
    EndianSwap((unsigned short)0, header.xOrigin);
    EndianSwap((unsigned short)0, header.yOrigin);
    EndianSwap((unsigned short)width, header.width);
    EndianSwap((unsigned short)height, header.height);
    header.pixelDepth = bpp;
    header.imageDescriptor = 0;
    file.write((char*)&header, sizeof(Header));
    
    // Write pixels
    for (unsigned i = 0; i < width * height; i++)
    {
        file.write((char*)&pixels[i*3+2], 1);
        file.write((char*)&pixels[i*3+1], 1);
        file.write((char*)&pixels[i*3], 1);
    }
    
    file.close();
    return true;
}
