
#ifndef TEXTURE_H
#define TEXTURE_H

#include "Image.h"

namespace DrageEngine
{
    class Texture
    {
        public:
            Texture();
            Texture(Image *image);
            Texture(unsigned id, int width, int height);
            ~Texture();
        
            unsigned Generate(Image *image);
        
            unsigned GetWidth() const;
            unsigned GetHeight() const;
            unsigned GetID() const;
        
        private:
            unsigned width;
            unsigned height;
            unsigned id;
    };
}

#endif
