
#ifndef IMAGEJPG_H
#define IMAGEJPG_H

#include "Image.h"

namespace DrageEngine
{
    class ImageJPG : public Image
    {
        public:
            ImageJPG();
            ImageJPG(const Image &other);
            ImageJPG(const std::string &filename);
            ~ImageJPG();
        
            bool Load(const std::string &filename);
        
        private:
            typedef struct
            {
                unsigned char bits;
                unsigned char code;
            } VLCCode;
        
            typedef struct
            {
                int cid;
                int ssx, ssy;
                int width, height;
                int stride;
                int qtsel;
                int actabsel, dctabsel;
                int dcpred;
                unsigned char *pixels;
            } Component;
        
            static const char ZZ[64];
            int m_blockWidth, m_blockHeight;
            Component comp[3];
            unsigned char qtab[4][64];
            VLCCode vlctab[4][65536];
            int buf, bufbits;
            int block[64];
            int rstinterval;
            unsigned char *rgb;
        
            unsigned char Clip(const int x);
            void RowIDCT(int* blk);
            void ColIDCT(const int* blk, unsigned char *out, int stride);
            int ShowBits(unsigned char* &pos, int bits);
            void SkipBits(unsigned char* &pos, int bits);
            int GetBits(unsigned char* &pos, int bits);
            void ByteAlign();
            unsigned short Decode16(unsigned char *pos);
            int DecodeLength(unsigned char* &pos);
            void SkipMarker(unsigned char* &pos);
            void DecodeSOF(unsigned char* &pos);
            void DecodeDHT(unsigned char* &pos);
            void DecodeDQT(unsigned char* &pos);
            void DecodeDRI(unsigned char* &pos);
            int GetVLC(unsigned char* &pos, VLCCode* vlc, unsigned char* code);
            void DecodeBlock(unsigned char* &pos, Component* c, unsigned char* out);
            void DecodeScan(unsigned char* &pos);
            void UpsampleH(Component* c);
            void UpsampleV(Component* c);
            void Convert();
            bool Decode(unsigned char* fileData, unsigned long fileSize);
    };
}

#endif
