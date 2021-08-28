
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
            ~Texture();
        
            unsigned Generate(Image *image);
        
            unsigned GetWidth() const;
            unsigned GetHeight() const;
            unsigned GetID() const;
            const std::string& GetName() const;
        
        private:
            std::string m_name;
            unsigned m_width;
            unsigned m_height;
            unsigned m_id;
    };
}

#endif
