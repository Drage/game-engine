
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <OpenGL/gl.h>
#include "Texture.h"

using namespace DrageEngine;

Texture::Texture()
{
    m_width = 0;
    m_height = 0;
    m_id = 0;
}

Texture::Texture(Image *image)
{
    m_width = 0;
    m_height = 0;
    m_id = 0;
    Generate(image);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

unsigned Texture::Generate(Image *image)
{
    m_width = image->GetWidth();
    m_height = image->GetHeight();
    m_name = image->GetName();
    
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    if (image->GetBPP() == 32)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->GetPixels());
    else if (image->GetBPP() == 24)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->GetPixels());
    else if (image->GetBPP() == 8)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, m_width, m_height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image->GetPixels());
    
    glGenerateMipmap(GL_TEXTURE_2D);
    
    return m_id;
}

unsigned Texture::GetWidth() const
{
    return m_width;
}

unsigned Texture::GetHeight() const
{
    return m_height;
}

unsigned Texture::GetID() const
{
    return m_id;
}

const std::string& Texture::GetName() const
{
    return m_name;
}
