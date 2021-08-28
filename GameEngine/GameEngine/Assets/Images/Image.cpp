
#include "Image.h"
#include "Debug.h"
#include "MathUtils.h"
#include "FileSystem.h"

using namespace DrageEngine;

Image::Image()
{
    width = 0;
    height = 0;
    bpp = 0;
    imageSize = 0;
    pixels = NULL;
}

Image::Image(const Image &other)
{
    *this = other;
}

Image::Image(unsigned char *pixels, unsigned width, unsigned height, unsigned bpp)
{
    width = width;
    height = height;
    bpp = bpp;
    imageSize = width * height * (bpp/8);
    this->pixels = new unsigned char[imageSize];
    
    if (pixels)
        memcpy(this->pixels, pixels, imageSize);
}

Image::~Image()
{
    if (pixels)
    {
        delete[] pixels;
        pixels = NULL;
    }
}

void Image::operator= (const Image &other)
{
    width = other.width;
    height = other.height;
    bpp = other.bpp;
    imageSize = other.imageSize;
    name = other.name;
    
    if (pixels)
        delete pixels;
    pixels = new unsigned char[imageSize];
    memcpy(pixels, other.pixels, imageSize);
}

bool Image::Load(const std::string &filename)
{
    name = GetFileName(filename);
    ERROR("Cannot load image as generic base type");
    return false;
}

bool Image::Save(const std::string &filePath)
{
    WARNING("Cannot save file. Image encoding not supported for this format.");
    return false;
}

unsigned Image::GetWidth() const
{
    return width;
}

unsigned Image::GetHeight() const
{
    return height;
}

unsigned Image::GetBPP() const
{
    return bpp;
}

unsigned long Image::GetSize() const
{
    return imageSize;
}

unsigned char* Image::GetPixels() const
{
    return pixels;
}

const std::string& Image::GetName() const
{
    return name;
}

bool Image::SetPixelColour(unsigned x, unsigned y, const Color &colour)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return false;
    
    if (bpp == 32)
    {
        unsigned char r, g, b, a;
        int position = (y * height + x) * (bpp/8);
        r = (unsigned char)Clamp<float>(colour.r * 255, 0, 255);
        g = (unsigned char)Clamp<float>(colour.g * 255, 0, 255);
        b = (unsigned char)Clamp<float>(colour.b * 255, 0, 255);
        a = (unsigned char)Clamp<float>(colour.a * 255, 0, 255);
        pixels[position]     = r;
        pixels[position + 1] = g;
        pixels[position + 2] = b;
        pixels[position + 3] = a;
    }
    else if (bpp == 24)
    {
        unsigned char r, g, b;
        int position = (y * height + x) * (bpp/8);
        r = (unsigned char)Clamp<float>(colour.r * 255, 0, 255);
        g = (unsigned char)Clamp<float>(colour.g * 255, 0, 255);
        b = (unsigned char)Clamp<float>(colour.b * 255, 0, 255);
        pixels[position]     = r;
        pixels[position + 1] = g;
        pixels[position + 2] = b;
    }
    else
    {
        unsigned char l;
        int position = (y * height + x) * (bpp/8);
        l = (unsigned char)Clamp<float>(colour.r * 255, 0, 255);
        pixels[position] = l;
    }
    return true;
}

bool Image::SetPixelColour(unsigned x, unsigned y, unsigned char r, unsigned char g, unsigned char b)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return false;
    
    int position = (y * height + x) * (bpp/8);
    pixels[position]     = Clamp<unsigned char>(r, 0, 255);
    pixels[position + 1] = Clamp<unsigned char>(g, 0, 255);
    pixels[position + 2] = Clamp<unsigned char>(b, 0, 255);
    
    return true;
}

Color Image::GetPixelColour(unsigned x, unsigned y)
{
    if (bpp == 32)
    {
        float r, g, b, a;
        int position = (y * height + x) * (bpp/8);
        r = pixels[position] / 255.0f;
        g = pixels[position + 1] / 255.0f;
        b = pixels[position + 2] / 255.0f;
        a = pixels[position + 3] / 255.0f;
        return Color(r, g, b, a);
    }
    else if (bpp == 24)
    {
        float r, g, b;
        int position = (y * height + x) * (bpp/8);
        r = pixels[position] / 255.0f;
        g = pixels[position + 1] / 255.0f;
        b = pixels[position + 2] / 255.0f;
        return Color(r, g, b);
    }
    else
    {
        float l;
        int position = (y * height + x) * (bpp/8);
        l = pixels[position] / 255.0f;
        return Color(l);
    }
}

bool Image::GetPixelColour(unsigned x, unsigned y, unsigned char *r, unsigned char *g, unsigned char *b)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return false;
    
    int position = (y * height + x) * (bpp/8);
    *r = pixels[position];
    *g = pixels[position + 1];
    *b = pixels[position + 2];
    return true;
}

void Image::EndianSwap(unsigned short x, char *result)
{
    result[0] = x & 0x00FF;
    result[1] = (x & 0xFF00) / 256;
}

void Image::EndianSwap(unsigned int x, char *result)
{
    result[0] = x & 0x000000FF;
    result[1] = (x & 0x0000FF00) / 256;
    result[2] = (x & 0x00FF0000) / 65536;
    result[3] = (x & 0xFF000000) / 16777216;
}

void Image::FlipVertical()
{
    unsigned char *tempBuffer = new unsigned char[imageSize];
    int increment = width * (bpp/8);
    unsigned char *buffer = tempBuffer + imageSize - increment;
    unsigned char *data = pixels;
    
    for (unsigned i = 0; i < height; i++)
    {
        memcpy(buffer, data, increment);
        buffer -= increment;
        data += increment;
    }
    
    delete pixels;
    pixels = tempBuffer;
}
