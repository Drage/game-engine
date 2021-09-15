
#ifndef FONT_H
#define FONT_H

#include <SDL2_ttf/SDL_ttf.h>
#include <string>
#include "Color.h"

namespace DrageEngine
{
    class Font
    {
        public:
            Font();
            ~Font();
            bool Load(const std::string &filename, int size);
            void RenderText(const std::string &text, const Color &color);
        
        private:        
            TTF_Font* font;
    };
}

#endif
