
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <OpenGL/gl.h>
#include "Texture.h"

using namespace DrageEngine;

Texture::Texture()
{
    width = 0;
    height = 0;
    id = 0;
}

Texture::Texture(Image *image)
{
    width = 0;
    height = 0;
    id = 0;
    Generate(image);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

unsigned Texture::Generate(Image *image)
{
    width = image->GetWidth();
    height = image->GetHeight();
    name = image->GetName();
    
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    if (image->GetBPP() == 32)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->GetPixels());
    else if (image->GetBPP() == 24)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->GetPixels());
    else if (image->GetBPP() == 8)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image->GetPixels());
    
    glGenerateMipmap(GL_TEXTURE_2D);
    
    return id;
}

unsigned Texture::GetWidth() const
{
    return width;
}

unsigned Texture::GetHeight() const
{
    return height;
}

unsigned Texture::GetID() const
{
    return id;
}

const std::string& Texture::GetName() const
{
    return name;
}
