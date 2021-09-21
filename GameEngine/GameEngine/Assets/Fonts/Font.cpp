
#include "Font.h"
#include "Debug.h"
#include <SDL_opengl.h>

using namespace DrageEngine;

Font::Font()
{
    font = NULL;
}

Font::~Font()
{
    TTF_CloseFont(font);
}

bool Font::Load(const std::string &filename, int size)
{
    font = TTF_OpenFont(filename.c_str(), size);
    
    if (!font)
        ERROR("Failed to load font " + filename + ": " + TTF_GetError());
    
    return font != NULL;
}

Texture* Font::Render(const std::string &text, const Color &color)
{
    SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color.ToSDLColor());
    if (!surface)
    {
        std::string error = "Failed to render text: ";
        error.append(TTF_GetError());
        ERROR(error);
        return NULL;
    }
    
    // Invert SDL surface in Y axis
    char* temp = new char[surface->pitch];
    for (int i = 0; i < surface->h / 2; i++)
    {
        char* row1 = (char*)surface->pixels + i * surface->pitch;
        char* row2 = (char*)surface->pixels + (surface->h - i - 1) * surface->pitch;
        memcpy(temp, row1, surface->pitch);
        memcpy(row1, row2, surface->pitch);
        memcpy(row2, temp, surface->pitch);
    }
    delete[] temp;
    
    // Convert to RGB
    SDL_Surface* rgbSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    SDL_BlitSurface(surface, NULL, rgbSurface, NULL);
    
    // Convert to OpenGL texture
    unsigned textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgbSurface->w, rgbSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbSurface->pixels);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    Texture *texture = new Texture(textureId, rgbSurface->w, rgbSurface->h);
    
    SDL_FreeSurface(surface);
    SDL_FreeSurface(rgbSurface);
    return texture;
}
