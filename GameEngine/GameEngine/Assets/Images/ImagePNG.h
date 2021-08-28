
#ifndef IMAGEPNG_H
#define IMAGEPNG_H

#include "Image.h"

namespace DrageEngine
{
    class ImagePNG : public Image
    {
        public:
            ImagePNG();
            ImagePNG(const Image &other);
            ImagePNG(const std::string &filename);
            bool Load(const std::string &filename);
        
        private:
            static const int signature[8];
            enum ColourType { GREYSCALE = 0, INDEXED = 3, TRUECOLOUR = 2, ALPHA = 4 };
            enum InterlaceMethod { NO_INTERLACE = 0, ADAM7 = 1 };
            enum Filter { NO_FILTER = 0, SUB = 1, UP = 2, AVERAGE = 3, PAETH = 4 };
        
            bool IsPNG(const unsigned char* data);
            void GetChunkInfo(unsigned char* chunk, unsigned &length, char type[], unsigned char* &data, unsigned &crc);
            bool ReadIHDR(unsigned char* data);
            void ReadPLTE(unsigned char* data);
            void ReadIDAT(unsigned char* data, unsigned length);
        
            bool Decompress();
        
            void ApplyFiltering();
            void FilterSub(unsigned char *scanline);
            void FilterUp(unsigned char *scanline, unsigned char *previous);
            void FilterAverage(unsigned char *scanline, unsigned char *previous);
            void FilterPaeth(unsigned char *scanline, unsigned char *previous);
            void ApplyPalette();
        
            bool readIHDR;
            unsigned char bitDepth;
            unsigned char colourType;
            unsigned char compression;
            unsigned char filter;
            unsigned char interlace;
        
            bool readPLTE;
            unsigned char *palette;
            unsigned char *pixelIndexes;
        
            unsigned char *dataStartPos;
            unsigned char *dataConcatenatePos;
        };
}

#endif
