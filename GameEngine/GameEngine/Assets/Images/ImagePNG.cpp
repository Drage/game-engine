
#include <fstream>
#include <zlib.h>
#include "ImagePNG.h"
#include "ArrayUtils.h"
#include "FileSystem.h"
#include "Debug.h"

using namespace DrageEngine;

const int ImagePNG::signature[] = { 137, 80, 78, 71, 13, 10, 26, 10 };

ImagePNG::ImagePNG()
{
    readIHDR = false;
    readPLTE = false;
    bitDepth = 0;
    colourType = 0;
    compression = 0;
    filter = 0;
    interlace = 0;
    dataStartPos = NULL;
}

ImagePNG::ImagePNG(const Image &other)
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

ImagePNG::ImagePNG(const std::string &filename)
{
    readIHDR = false;
    readPLTE = false;
    bitDepth = 0;
    colourType = 0;
    compression = 0;
    filter = 0;
    interlace = 0;
    dataStartPos = NULL;
    Load(filename);
}

bool ImagePNG::Load(const std::string &filename)
{
    name = GetFileName(filename);
    unsigned long fileSize;
    unsigned char* fileData = NULL;
    std::ifstream file;
    
    try
    {
        // Open the file
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
        
        // Check file is PNG
        if (!IsPNG(fileData))
        {
            ERROR("Failed to load image - Invalid file format: " + filename);
            throw std::exception();
        }
        
        // Read data
        unsigned char* chunk = fileData + Array::Length(signature);
        unsigned length;
        char typeCode[4];
        unsigned char* chunkData;
        unsigned crc;
        bool fileEnd = false;
        
        while (!fileEnd)
        {
            GetChunkInfo(chunk, length, typeCode, chunkData, crc);
            
            if (memcmp(typeCode, "IHDR", 4) == 0)
            {
                if (!ReadIHDR(chunkData))
                {
                    ERROR("Failed to load image - Encoding not supported: " + filename);
                    throw std::exception();
                }
            }
            else if (memcmp(typeCode, "PLTE", 4) == 0)
            {
                ReadPLTE(chunkData);
            }
            else if (memcmp(typeCode, "IDAT", 4) == 0)
            {
                ReadIDAT(chunkData, length);
            }
            else if (memcmp(typeCode, "IEND", 4) == 0)
            {
                fileEnd = true;
            }
            
            chunk += length + 12;
        }
        
        // Apply zLib inflate to compressed data
        if (!Decompress())
        {
            ERROR("Failed to load image - Could not decompress: " + filename);
            throw std::exception();
        }
        
        // Reconstruct image from palette
        if (colourType == INDEXED)
            ApplyPalette();
        
        // Reconstruct image from scanline filtering
        ApplyFiltering();
        
        // Flip row order so image is right way up
        FlipVertical();
        
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

bool ImagePNG::IsPNG(const unsigned char* data)
{
    try
    {
        for (int i = 0; i < Array::Length(signature); i++)
        {
            if (data[i] != signature[i])
                return false;
        }
        return true;
    }
    catch (std::exception&)
    {
        return false;
    }
}

void ImagePNG::GetChunkInfo(unsigned char* chunk, unsigned &length, char type[], unsigned char* &data, unsigned &crc)
{
    length = chunk[0];
    length = (length << 8) + chunk[1];
    length = (length << 8) + chunk[2];
    length = (length << 8) + chunk[3];
    
    memcpy(type, &chunk[4], 4);
    
    data = chunk + 8;
    
    if (memcmp(type, "IEND", 4) != 0)
        memcpy(&crc, &chunk[length + 8], 4);
}

bool ImagePNG::ReadIHDR(unsigned char* data)
{
    // Get image dimensions
    width = data[0];
    width = (width << 8) + data[1];
    width = (width << 8) + data[2];
    width = (width << 8) + data[3];
    
    height = data[4];
    height = (height << 8) + data[5];
    height = (height << 8) + data[6];
    height = (height << 8) + data[7];
    
    // Get other metadata
    bitDepth = data[8];
    colourType = data[9];
    compression = data[10];
    filter = data[11];
    interlace = data[12];
    
    // Don't load unsupported file formats
    if (colourType == GREYSCALE + ALPHA || bitDepth != 8 || interlace != NO_INTERLACE || compression != 0 || filter != 0)
        return false;
    
    // Calculate number of bits-per-pixel
    switch (colourType)
    {
        case GREYSCALE:
            bpp = bitDepth;
            break;
        case TRUECOLOUR:
            bpp = 3 * bitDepth;
            break;
        case INDEXED:
            bpp = 24;
            break;
        case (GREYSCALE + ALPHA):
            bpp = 2 * bitDepth;
            break;
        case (TRUECOLOUR + ALPHA):
            bpp = 4 * bitDepth;
            break;
    }
    
    // Allocate memory for pixel data: image size * bytes-per-pixel + one extra byte per scanline for filter type
    imageSize = (width * height * bpp/8) + height;
    pixels = new unsigned char[imageSize];
    
    readIHDR = true;
    
    return pixels != NULL;
}

void ImagePNG::ReadPLTE(unsigned char* data)
{
    palette = data;
    readPLTE = true;
}

void ImagePNG::ReadIDAT(unsigned char* data, unsigned length)
{
    if (dataStartPos == NULL)
    {
        dataStartPos = data;
        dataConcatenatePos = dataStartPos + length;
    }
    else
    {
        memcpy(dataConcatenatePos, data, length);
        dataConcatenatePos += length;
    }
}

bool ImagePNG::Decompress()
{
    // Use zlib to decompress image data
    return uncompress(pixels, &imageSize, dataStartPos, dataConcatenatePos - dataStartPos) == Z_OK;
}

void ImagePNG::ApplyFiltering()
{
    unsigned char *currentRow = pixels;
    unsigned char *lastRow = pixels + 1;
    unsigned char *compactedPosition = pixels;
    
    // Apply filter to each scanline - each scanline can have a different filter
    for (unsigned i = 0; i < height; i++)
    {
        switch (currentRow[0])
        {
            case NO_FILTER:
                break;
            case SUB:
                FilterSub(currentRow + 1);
                break;
            case UP:
                FilterUp(currentRow + 1, lastRow);
                break;
            case AVERAGE:
                FilterAverage(currentRow + 1, lastRow);
                break;
            case PAETH:
                FilterPaeth(currentRow + 1, lastRow);
                break;
        }
        
        lastRow = compactedPosition;
        
        // Compact data to remove filter type bytes
        memcpy(compactedPosition, currentRow + 1, width * bpp/8);
        compactedPosition += width * bpp/8;
        
        currentRow += width * bpp/8 + 1;
        
        imageSize = width * height * (bpp/8);
    }
}

void ImagePNG::FilterSub(unsigned char *scanline)
{
    // Store last pixel's data, init to 0
    unsigned char *last = new unsigned char[bpp/8];
    for (unsigned i = 0; i < bpp/8; i++)
        last[i] = 0;
    
    // Add pixel to the left to current pixel
    for (unsigned i = 0; i < width * bpp/8; i++)
    {
        int lastIndex = i % (bpp/8);
        scanline[i] = (scanline[i] + last[lastIndex]) % 256;
        last[lastIndex] = scanline[i];
    }
    
    delete[] last;
}

void ImagePNG::FilterUp(unsigned char *scanline, unsigned char *previous)
{
    if (scanline != previous)
    {
        // Add pixel above to current pixel
        for (unsigned i = 0; i < width * bpp/8; i++)
        {
            scanline[i] = (scanline[i] + previous[i]) % 256;
        }
    }
}

void ImagePNG::FilterAverage(unsigned char *scanline, unsigned char *previous)
{
    // Store last pixel's data, init to 0
    unsigned char *last = new unsigned char[bpp/8];
    for (unsigned i = 0; i < bpp/8; i++)
        last[i] = 0;
    
    // Add the average of the pixel to the left and the pixel above to the current pixel
    if (scanline != previous)
    {
        for (unsigned i = 0; i < width * bpp/8; i++)
        {
            scanline[i] = (scanline[i] + (last[i % (bpp/8)] + previous[i]) / 2) % 256;
            last[i % (bpp/8)] = scanline[i];
        }
    }
    else // If first scanline, assume previous row was all 0s
    {
        for (unsigned i = 0; i < width * bpp/8; i++)
        {
            scanline[i] = (scanline[i] + last[i % (bpp/8)] / 2) % 256;
            last[i % (bpp/8)] = scanline[i];
        }
    }
    
    delete[] last;
}

void ImagePNG::FilterPaeth(unsigned char *scanline, unsigned char *previous)
{
    int a, b, c;
    int p, pa, pb, pc;
    
    // Store last pixel's data, init to 0
    unsigned char *last = new unsigned char[bpp/8];
    for (unsigned i = 0; i < bpp/8; i++)
        last[i] = 0;
    
    // Store last pixel of the previous scanline's data, init to 0
    unsigned char *prevLast = new unsigned char[bpp/8];
    for (unsigned i = 0; i < bpp/8; i++)
        prevLast[i] = 0;
    
    for (unsigned i = 0; i < width * bpp/8; i++)
    {
        // get index of current pixel component (r,g,b,a...)
        int lastIndex = i % (bpp/8);
        
        a = last[lastIndex]; // a is pixel to the left
        
        if (scanline != previous)
            b = previous[i]; // b is pixel above
        else
            b = 0;
        
        c = prevLast[lastIndex]; // c is pixel above and to the left
        
        // Paeth calculates each pixel by trying to predict which of the adjacent pixels
        // will be most similar to the current one
        p = a + b - c;
        pa = abs(p - a);
        pb = abs(p - b);
        pc = abs(p - c);
        
        if (pa <= pb && pa <= pc)
            scanline[i] = (scanline[i] + a) % 256;
        else if (pb <= pc)
            scanline[i] = (scanline[i] + b) % 256;
        else
            scanline[i] = (scanline[i] + c) % 256;
        
        // Update last pixel buffers
        last[lastIndex] = scanline[i];
        prevLast[lastIndex] = b;
    }
    
    delete[] last;
    delete[] prevLast;
}

void ImagePNG::ApplyPalette()
{
    if (!readPLTE)
        return;
    
    // Allocate memory for palette indexes
    pixelIndexes = new unsigned char[width * height + height];
    memcpy(pixelIndexes, pixels, width * height + height);
    
    unsigned char *pixelPosition = pixels;
    
    for (unsigned i = 0; i < width * height + height; i++)
    {
        if (i % (width + 1) == 0)
        {
            // Copy filter type byte
            memcpy(pixelPosition, &pixelIndexes[i], 1);
            pixelPosition++;
        }
        else
        {
            // Copy RBG bytes from palette
            memcpy(pixelPosition, palette + pixelIndexes[i] * 3, 3);
            pixelPosition += 3;
        }
    }
    
    // Free pixel index data
    delete[] pixelIndexes;
}
