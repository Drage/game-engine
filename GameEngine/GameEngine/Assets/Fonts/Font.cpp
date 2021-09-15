
#include "Font.h"
#include "Debug.h"
#include <SDL2/SDL_opengl.h>

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

void Font::RenderText(const std::string &text, const Color &color)
{
    SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color.ToSDLColor());
    
    if (!surface)
    {
        std::string error = "Failed to render text: ";
        error.append(TTF_GetError());
        ERROR(error);
        return;
    }

    unsigned texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
    
    SDL_FreeSurface(surface);
}
