
#ifndef IMAGEBMP_H
#define IMAGEBMP_H

#include "Image.h"

namespace DrageEngine
{
    class ImageBMP : public Image
    {
        public:
            ImageBMP();
            ImageBMP(const Image &other);
            ImageBMP(const std::string &filename);
            bool Load(const std::string &filename);
            bool Save(const std::string &filePath);
        
        private:
            bool ReadHeader(unsigned char *data);
            bool LoadData(unsigned char *data);
            void BGRtoRGB();
        
            unsigned m_pixelDataOffset;
        
            typedef struct Header
            {
                char signature[2];
                char fileSize[4];
                char padding[4];
                char imageOffset[4];
                char bitmapInfoHeaderSize[4];
                char width[4];
                char height[4];
                char numPlanes[2];
                char pixelDepth[2];
                char compressionType[4];
                char imageSize[4];
                char horizResolution[4];
                char vertResolution[4];
                char importantColours[4];
            } Header;
        };
}

#endif
