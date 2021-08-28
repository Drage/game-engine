
#ifndef IMAGETGA_H
#define IMAGETGA_H

#include "Image.h"

namespace DrageEngine
{
    class ImageTGA : public Image
    {
        public:
            ImageTGA();
            ImageTGA(const Image &other);
            ImageTGA(const std::string &filename);
            bool Load(const std::string &filename);
            bool Save(const std::string &filePath);
        
        private:
            enum EncodingType { RAW_RGB_ENCODING = 2, RLE_RGB_ENCODING = 10 };
        
            bool ReadHeader(unsigned char *data);
            void LoadRawData(unsigned char *data);
            void LoadRLEData(unsigned char *data);
            void BGRtoRGB();
        
            char m_encoding;
        
            typedef struct
            {
                char idLength;
                char colourMapType;
                char imageType;
                char colourMap[5];
                char xOrigin[2];
                char yOrigin[2];
                char width[2];
                char height[2];
                char pixelDepth;
                char imageDescriptor;
            } Header;
    };
}

#endif
