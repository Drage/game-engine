
#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include "Color.h"

namespace DrageEngine
{
    class Image
    {
        public:
            Image();
            Image(const Image &other);
            Image(unsigned char *pixels, unsigned width, unsigned height, unsigned bpp);
            virtual ~Image();
        
            void operator= (const Image &other);
        
            virtual bool Load(const std::string &filename);
            virtual bool Save(const std::string &filePath);
        
            unsigned GetWidth() const;
            unsigned GetHeight() const;
            unsigned GetBPP() const;
            unsigned long GetSize() const;
            unsigned char* GetPixels() const;
            const std::string& GetName() const;
        
            bool SetPixelColour(unsigned x, unsigned y, const Color &colour);
            bool SetPixelColour(unsigned x, unsigned y, unsigned char r, unsigned char g, unsigned char b);
            Color GetPixelColour(unsigned x, unsigned y);
            bool GetPixelColour(unsigned x, unsigned y, unsigned char *r, unsigned char *g, unsigned char *b);
        
        protected:
            static void EndianSwap(unsigned short x, char *result);
            static void EndianSwap(unsigned int x, char *result);
            void FlipVertical();
        
            std::string name;
            unsigned width;
            unsigned height;
            unsigned bpp;
            unsigned char *pixels;
            unsigned long imageSize;
    };
}

#endif
