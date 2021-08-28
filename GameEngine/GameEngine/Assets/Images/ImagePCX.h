
#ifndef IMAGEPCX_H
#define IMAGEPCX_H

#include "Image.h"

namespace DrageEngine
{
    class ImagePCX : public Image
    {
        public:
            ImagePCX();
            ImagePCX(const Image &other);
            ImagePCX(const std::string &filename);
            bool Load(const std::string &filename);
        
        private:
            struct Header
            {
                char manufacturer, version, encoding, bitsPerPixel;
                short int xMin, yMin, xMax, yMax;
                char other[116];
            } m_header;
        
            static const int PALETTE_SIZE = 768;
        
            bool ReadHeader(unsigned char *data);
            bool LoadData(unsigned char *data);
        
            unsigned long m_fileSize;
    };
}

#endif
