
#include "Image.h"
#include "Debug.h"
#include "MathUtils.h"
#include "FileSystem.h"

using namespace DrageEngine;

Image::Image()
{
    m_width = 0;
    m_height = 0;
    m_bpp = 0;
    m_imageSize = 0;
    m_pixels = NULL;
}

Image::Image(const Image &other)
{
    *this = other;
}

Image::Image(unsigned char *pixels, unsigned width, unsigned height, unsigned bpp)
{
    m_width = width;
    m_height = height;
    m_bpp = bpp;
    m_imageSize = width * height * (m_bpp/8);
    m_pixels = new unsigned char[m_imageSize];
    
    if (pixels)
        memcpy(m_pixels, pixels, m_imageSize);
}

Image::~Image()
{
    if (m_pixels)
    {
        delete[] m_pixels;
        m_pixels = NULL;
    }
}

void Image::operator= (const Image &other)
{
    m_width = other.m_width;
    m_height = other.m_height;
    m_bpp = other.m_bpp;
    m_imageSize = other.m_imageSize;
    m_name = other.m_name;
    
    if (m_pixels)
        delete m_pixels;
    m_pixels = new unsigned char[m_imageSize];
    memcpy(m_pixels, other.m_pixels, m_imageSize);
}

bool Image::Load(const std::string &filename)
{
    m_name = GetFileName(filename);
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
    return m_width;
}

unsigned Image::GetHeight() const
{
    return m_height;
}

unsigned Image::GetBPP() const
{
    return m_bpp;
}

unsigned long Image::GetSize() const
{
    return m_imageSize;
}

unsigned char* Image::GetPixels() const
{
    return m_pixels;
}

const std::string& Image::GetName() const
{
    return m_name;
}

bool Image::SetPixelColour(unsigned x, unsigned y, const Color &colour)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return false;
    
    if (m_bpp == 32)
    {
        unsigned char r, g, b, a;
        int position = (y * m_height + x) * (m_bpp/8);
        r = (unsigned char)Clamp<float>(colour.r * 255, 0, 255);
        g = (unsigned char)Clamp<float>(colour.g * 255, 0, 255);
        b = (unsigned char)Clamp<float>(colour.b * 255, 0, 255);
        a = (unsigned char)Clamp<float>(colour.a * 255, 0, 255);
        m_pixels[position]     = r;
        m_pixels[position + 1] = g;
        m_pixels[position + 2] = b;
        m_pixels[position + 3] = a;
    }
    else if (m_bpp == 24)
    {
        unsigned char r, g, b;
        int position = (y * m_height + x) * (m_bpp/8);
        r = (unsigned char)Clamp<float>(colour.r * 255, 0, 255);
        g = (unsigned char)Clamp<float>(colour.g * 255, 0, 255);
        b = (unsigned char)Clamp<float>(colour.b * 255, 0, 255);
        m_pixels[position]     = r;
        m_pixels[position + 1] = g;
        m_pixels[position + 2] = b;
    }
    else
    {
        unsigned char l;
        int position = (y * m_height + x) * (m_bpp/8);
        l = (unsigned char)Clamp<float>(colour.r * 255, 0, 255);
        m_pixels[position] = l;
    }
    return true;
}

bool Image::SetPixelColour(unsigned x, unsigned y, unsigned char r, unsigned char g, unsigned char b)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return false;
    
    int position = (y * m_height + x) * (m_bpp/8);
    m_pixels[position]     = Clamp<unsigned char>(r, 0, 255);
    m_pixels[position + 1] = Clamp<unsigned char>(g, 0, 255);
    m_pixels[position + 2] = Clamp<unsigned char>(b, 0, 255);
    
    return true;
}

Color Image::GetPixelColour(unsigned x, unsigned y)
{
    if (m_bpp == 32)
    {
        float r, g, b, a;
        int position = (y * m_height + x) * (m_bpp/8);
        r = m_pixels[position] / 255.0f;
        g = m_pixels[position + 1] / 255.0f;
        b = m_pixels[position + 2] / 255.0f;
        a = m_pixels[position + 3] / 255.0f;
        return Color(r, g, b, a);
    }
    else if (m_bpp == 24)
    {
        float r, g, b;
        int position = (y * m_height + x) * (m_bpp/8);
        r = m_pixels[position] / 255.0f;
        g = m_pixels[position + 1] / 255.0f;
        b = m_pixels[position + 2] / 255.0f;
        return Color(r, g, b);
    }
    else
    {
        float l;
        int position = (y * m_height + x) * (m_bpp/8);
        l = m_pixels[position] / 255.0f;
        return Color(l);
    }
}

bool Image::GetPixelColour(unsigned x, unsigned y, unsigned char *r, unsigned char *g, unsigned char *b)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return false;
    
    int position = (y * m_height + x) * (m_bpp/8);
    *r = m_pixels[position];
    *g = m_pixels[position + 1];
    *b = m_pixels[position + 2];
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
    unsigned char *tempBuffer = new unsigned char[m_imageSize];
    int increment = m_width * (m_bpp/8);
    unsigned char *buffer = tempBuffer + m_imageSize - increment;
    unsigned char *data = m_pixels;
    
    for (unsigned i = 0; i < m_height; i++)
    {
        memcpy(buffer, data, increment);
        buffer -= increment;
        data += increment;
    }
    
    delete m_pixels;
    m_pixels = tempBuffer;
}
