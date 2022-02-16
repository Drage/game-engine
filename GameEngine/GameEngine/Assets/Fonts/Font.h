
#ifndef FONT_H
#define FONT_H

//#include <SDL2_ttf/SDL_ttf.h>
#include <string>
#include "Color.h"
#include "Texture.h"

namespace DrageEngine
{
    class Font
    {
        public:
            Font();
            ~Font();
            bool Load(const std::string &filename, int size);
            Texture* Render(const std::string &text, const Color &color);
        
        private:
            //TTF_Font* font;
    };
}

#endif
