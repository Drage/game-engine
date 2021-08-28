
#include <fstream>
#include "ImageJPG.h"
#include "Debug.h"
#include "FileSystem.h"

using namespace DrageEngine;

ImageJPG::ImageJPG()
{
    
}

ImageJPG::ImageJPG(const Image &other)
{
    m_width = other.GetWidth();
    m_height = other.GetHeight();
    m_bpp = other.GetBPP();
    m_imageSize = other.GetSize();
    
    if (m_pixels)
        delete m_pixels;
    m_pixels = new unsigned char[m_imageSize];
    memcpy(m_pixels, other.GetPixels(), m_imageSize);
}

ImageJPG::ImageJPG(const std::string &filename)
{
    Load(filename);
}

ImageJPG::~ImageJPG()
{
    for (int i = 0; i < 3; i++)
    {
        if (comp[i].pixels)
            free(comp[i].pixels);
    }
    
    if (rgb)
        free(rgb);
}

bool ImageJPG::Load(const std::string &filename)
{
    m_name = GetFileName(filename);
    unsigned long fileSize;
    unsigned char* fileData = NULL;
    std::ifstream file;
    
    try
    {
        file.open(filename, std::ios::binary);
        
        if (!file.is_open())
        {
            ERROR("Failed to load image - Could not open file: " + filename);
            throw std::exception();
        }
        
        // Get file size
        file.seekg(0, std::ios_base::end);
        fileSize = (long)file.tellg();
        file.seekg(0, std::ios_base::beg);
        
        // Allocate memory
        fileData = new unsigned char[fileSize];
        
        if (fileData == NULL)
        {
            ERROR("Failed to load image - Insufficient memory: " + filename);
            throw std::exception();
        }
        
        // Read the file into memory
        file.read((char*)fileData, fileSize);
        file.close();
        
        // Decode Jpeg data
        if (!Decode(fileData, fileSize))
        {
            ERROR("Failed to load image - Could not decode: " + filename);
            throw std::exception();
        }
        
        // Free temporary storage
        delete[] fileData;
        return true;
    }
    catch (std::exception&)
    {
        if (file.is_open())
            file.close();
        
        if (fileData)
            delete[] fileData;
        
        return false;
    }
}

const char ImageJPG::ZZ[64] =
{
    0,  1,  8,  16,  9,  2,  3, 10,
    17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63,
};

unsigned char ImageJPG::Clip(const int x)
{
    return (x < 0) ? 0 : ((x > 0xFF) ? 0xFF : (unsigned char) x);
}

#define W1 2841
#define W2 2676
#define W3 2408
#define W5 1609
#define W6 1108
#define W7 565

void ImageJPG::RowIDCT(int* blk)
{
    int x0, x1, x2, x3, x4, x5, x6, x7, x8;
    if (!((x1 = blk[4] << 11)
          | (x2 = blk[6])
          | (x3 = blk[2])
          | (x4 = blk[1])
          | (x5 = blk[7])
          | (x6 = blk[5])
          | (x7 = blk[3])))
    {
        blk[0] = blk[1] = blk[2] = blk[3] = blk[4] = blk[5] = blk[6] = blk[7] = blk[0] << 3;
        return;
    }
    x0 = (blk[0] << 11) + 128;
    x8 = W7 * (x4 + x5);
    x4 = x8 + (W1 - W7) * x4;
    x5 = x8 - (W1 + W7) * x5;
    x8 = W3 * (x6 + x7);
    x6 = x8 - (W3 - W5) * x6;
    x7 = x8 - (W3 + W5) * x7;
    x8 = x0 + x1;
    x0 -= x1;
    x1 = W6 * (x3 + x2);
    x2 = x1 - (W2 + W6) * x2;
    x3 = x1 + (W2 - W6) * x3;
    x1 = x4 + x6;
    x4 -= x6;
    x6 = x5 + x7;
    x5 -= x7;
    x7 = x8 + x3;
    x8 -= x3;
    x3 = x0 + x2;
    x0 -= x2;
    x2 = (181 * (x4 + x5) + 128) >> 8;
    x4 = (181 * (x4 - x5) + 128) >> 8;
    blk[0] = (x7 + x1) >> 8;
    blk[1] = (x3 + x2) >> 8;
    blk[2] = (x0 + x4) >> 8;
    blk[3] = (x8 + x6) >> 8;
    blk[4] = (x8 - x6) >> 8;
    blk[5] = (x0 - x4) >> 8;
    blk[6] = (x3 - x2) >> 8;
    blk[7] = (x7 - x1) >> 8;
}

void ImageJPG::ColIDCT(const int* blk, unsigned char *out, int stride)
{
    int x0, x1, x2, x3, x4, x5, x6, x7, x8;
    if (!((x1 = blk[8 * 4] << 8)
          | (x2 = blk[8 * 6])
          | (x3 = blk[8 * 2])
          | (x4 = blk[8 * 1])
          | (x5 = blk[8 * 7])
          | (x6 = blk[8 * 5])
          | (x7 = blk[8 * 3])))
    {
        x1 = Clip(((blk[0] + 32) >> 6) + 128);
        for (x0 = 8; x0; x0--)
        {
            *out = (unsigned char) x1;
            out += stride;
        }
        return;
    }
    x0 = (blk[0] << 8) + 8192;
    x8 = W7 * (x4 + x5) + 4;
    x4 = (x8 + (W1 - W7) * x4) >> 3;
    x5 = (x8 - (W1 + W7) * x5) >> 3;
    x8 = W3 * (x6 + x7) + 4;
    x6 = (x8 - (W3 - W5) * x6) >> 3;
    x7 = (x8 - (W3 + W5) * x7) >> 3;
    x8 = x0 + x1;
    x0 -= x1;
    x1 = W6 * (x3 + x2) + 4;
    x2 = (x1 - (W2 + W6) * x2) >> 3;
    x3 = (x1 + (W2 - W6) * x3) >> 3;
    x1 = x4 + x6;
    x4 -= x6;
    x6 = x5 + x7;
    x5 -= x7;
    x7 = x8 + x3;
    x8 -= x3;
    x3 = x0 + x2;
    x0 -= x2;
    x2 = (181 * (x4 + x5) + 128) >> 8;
    x4 = (181 * (x4 - x5) + 128) >> 8;
    *out = Clip(((x7 + x1) >> 14) + 128);  out += stride;
    *out = Clip(((x3 + x2) >> 14) + 128);  out += stride;
    *out = Clip(((x0 + x4) >> 14) + 128);  out += stride;
    *out = Clip(((x8 + x6) >> 14) + 128);  out += stride;
    *out = Clip(((x8 - x6) >> 14) + 128);  out += stride;
    *out = Clip(((x0 - x4) >> 14) + 128);  out += stride;
    *out = Clip(((x3 - x2) >> 14) + 128);  out += stride;
    *out = Clip(((x7 - x1) >> 14) + 128);
}

int ImageJPG::ShowBits(unsigned char* &pos, int bits)
{
    unsigned char newbyte;
    if (!bits) return 0;
    while (bufbits < bits)
    {
        newbyte = *pos++;
        bufbits += 8;
        buf = (buf << 8) | newbyte;
        if (newbyte == 0xFF)
        {
            unsigned char marker = *pos++;
            switch (marker)
            {
                case 0x00:
                case 0xFF:
                case 0xD9:
                    break;
                default:
                    if ((marker & 0xF8) != 0xD0)
                        throw std::exception();
                    else
                    {
                        buf = (buf << 8) | marker;
                        bufbits += 8;
                    }
            }
        }
    }
    return (buf >> (bufbits - bits)) & ((1 << bits) - 1);
}

void ImageJPG::SkipBits(unsigned char* &pos, int bits)
{
    if (bufbits < bits)
        (void) ShowBits(pos, bits);
    bufbits -= bits;
}

int ImageJPG::GetBits(unsigned char* &pos, int bits)
{
    int res = ShowBits(pos, bits);
    SkipBits(pos, bits);
    return res;
}

void ImageJPG::ByteAlign()
{
    bufbits &= 0xF8;
}

unsigned short ImageJPG::Decode16(unsigned char *pos)
{
    return (pos[0] << 8) | pos[1];
}

int ImageJPG::DecodeLength(unsigned char* &pos)
{
    int length = Decode16(pos);
    pos += 2;
    return length - 2;
}

void ImageJPG::SkipMarker(unsigned char* &pos)
{
    pos += DecodeLength(pos);
}

void ImageJPG::DecodeSOF(unsigned char* &pos)
{
    int i, ssxmax = 0, ssymax = 0;
    Component* c;
    int length = DecodeLength(pos);
    unsigned char *end = pos + length;
    
    if (length < 9)
        throw std::exception();
    
    if (pos[0] != 8)
        throw std::exception();
    
    m_height = Decode16(pos+1);
    m_width = Decode16(pos+3);
    
    int ncomp = pos[5];
    switch (ncomp)
    {
        case 1:
        case 3:
            break;
        default:
            throw std::exception();
    }
    m_bpp = ncomp == 3 ? 24 : 8;
    m_imageSize = m_width * m_height * ncomp;
    
    pos += 6;
    
    if (length < (ncomp * 3))
        throw std::exception();
    
    for (i = 0, c = comp; i < ncomp; i++, c++)
    {
        c->cid = pos[0];
        if ((!(c->ssx = pos[1] >> 4)
             || (c->ssx & (c->ssx - 1))
             || (!(c->ssy = pos[1] & 15))
             || (c->ssy & (c->ssy - 1))
             || ((c->qtsel = pos[2]) & 0xFC)))
            throw std::exception();
        
        pos += 3;
        if (c->ssx > ssxmax) ssxmax = c->ssx;
        if (c->ssy > ssymax) ssymax = c->ssy;
    }
            
    if (ncomp == 1)
    {
        c = comp;
        c->ssx = c->ssy = ssxmax = ssymax = 1;
    }
            
    int mbsizex = ssxmax << 3;
    int mbsizey = ssymax << 3;
    m_blockWidth = (m_width + mbsizex - 1) / mbsizex;
    m_blockHeight = (m_height + mbsizey - 1) / mbsizey;
            
    for (i = 0, c = comp; i < ncomp; i++, c++)
    {
        c->width = (m_width * c->ssx + ssxmax - 1) / ssxmax;
        c->stride = (c->width + 7) & 0x7FFFFFF8;
        c->height = (m_height * c->ssy + ssymax - 1) / ssymax;
        c->stride = m_blockWidth * mbsizex * c->ssx / ssxmax;
        
        if (((c->width < 3) && (c->ssx != ssxmax)) || ((c->height < 3) && (c->ssy != ssymax)))
            throw std::exception();
        
        if (!(c->pixels = (unsigned char*)malloc(c->stride * (m_blockHeight * mbsizey * c->ssy / ssymax))))
            throw std::exception();
    }
    
    if (ncomp == 3) {
        rgb = (unsigned char*)malloc(m_width * m_height * ncomp);
        if (!rgb)
            throw std::exception();
    }
    
    pos = end;
}

void ImageJPG::DecodeDHT(unsigned char* &pos)
{
    int codelen, currcnt, remain, spread, i, j;
    VLCCode *vlc;
    static unsigned char counts[16];
    
    int length = DecodeLength(pos);
    while (length >= 17)
    {
        i = pos[0];
        
        if ((i & 0xEC) || (i & 0x02))
            throw std::exception();

        i = (i | (i >> 3)) & 3;
        
        for (codelen = 1; codelen <= 16; codelen++)
            counts[codelen - 1] = pos[codelen];
        
        pos += 17;
        length -= 17;
        
        vlc = &vlctab[i][0];
        remain = spread = 65536;
        for (codelen = 1; codelen <= 16; codelen++)
        {
            spread >>= 1;
            currcnt = counts[codelen - 1];
            
            if (!currcnt)
                continue;
            
            if (length < currcnt)
                throw std::exception();
            
            remain -= currcnt << (16 - codelen);
            
            if (remain < 0)
                throw std::exception();
            
            for (i = 0; i < currcnt; i++)
            {
                unsigned char code = pos[i];
                for (j = spread; j; j--)
                {
                    vlc->bits = (unsigned char) codelen;
                    vlc->code = code;
                    vlc++;
                }
            }
            pos += currcnt;
            length -= currcnt;
        }
        while (remain--)
        {
            vlc->bits = 0;
            vlc++;
        }
    }
}

void ImageJPG::DecodeDQT(unsigned char* &pos)
{
    int i;
    unsigned char *t;
    
    int length = DecodeLength(pos);
    while (length >= 65)
    {
        i = pos[0];
        
        if (i & 0xFC)
            throw std::exception();
        
        t = &qtab[i][0];
        
        for (i = 0; i < 64; i++)
            t[i] = pos[i + 1];
        
        pos += 65;
        length -= 65;
    }
}

void ImageJPG::DecodeDRI(unsigned char* &pos)
{
    int length = DecodeLength(pos);
    
    if (length < 2)
        throw std::exception();
    
    rstinterval = Decode16(pos);
    pos += length;
}

int ImageJPG::GetVLC(unsigned char* &pos, VLCCode* vlc, unsigned char* code)
{
    int value = ShowBits(pos, 16);
    int bits = vlc[value].bits;
    
    if (!bits)
        throw std::exception();
    
    SkipBits(pos, bits);
    
    value = vlc[value].code;
    
    if (code)
        *code = (unsigned char) value;
    
    bits = value & 15;
    
    if (!bits)
        return 0;
    
    value = GetBits(pos, bits);
    
    if (value < (1 << (bits - 1)))
        value += ((-1) << bits) + 1;
    
    return value;
}

void ImageJPG::DecodeBlock(unsigned char* &pos, Component* c, unsigned char* out)
{
    unsigned char code = 0;
    int value, coef = 0;
    memset(block, 0, sizeof(block));
    
    c->dcpred += GetVLC(pos, &vlctab[c->dctabsel][0], NULL);
    block[0] = (c->dcpred) * qtab[c->qtsel][0];
    do
    {
        value = GetVLC(pos, &vlctab[c->actabsel][0], &code);
        
        if (!code)
            break;
        
        if (!(code & 0x0F) && (code != 0xF0))
            throw std::exception();
        
        coef += (code >> 4) + 1;
        
        if (coef > 63)
            throw std::exception();
        
        block[(int) ZZ[coef]] = value * qtab[c->qtsel][coef];
    }
    while (coef < 63);
    
    for (coef = 0; coef < 64; coef += 8)
        RowIDCT(&block[coef]);
    
    for (coef = 0; coef < 8; coef++)
        ColIDCT(&block[coef], &out[coef], c->stride);
}

void ImageJPG::DecodeScan(unsigned char* &pos)
{
    int i, mbx, mby, sbx, sby;
    int rstcount = rstinterval, nextrst = 0;
    Component* c;
    
    int length = DecodeLength(pos);
    int ncomp = m_bpp / 8;
    if (length < (4 + 2 * ncomp) || pos[0] != ncomp)
        throw std::exception();
    unsigned char *end = pos + length;
    
    pos += 1;
    
    for (i = 0, c = comp; i < ncomp; i++, c++)
    {
        if (pos[0] != c->cid || pos[1] & 0xEE)
            throw std::exception();
        
        c->dctabsel = pos[1] >> 4;
        c->actabsel = (pos[1] & 1) | 2;
        pos += 2;
    }
    
    if (pos[0] || (pos[1] != 63) || pos[2])
        throw std::exception();
    
    pos = end;
    
    for (mbx = mby = 0;;)
    {
        for (i = 0, c = comp; i < ncomp; i++, c++)
        {
            for (sby = 0; sby < c->ssy; sby++)
            {
                for (sbx = 0; sbx < c->ssx; sbx++)
                    DecodeBlock(pos, c, &c->pixels[((mby * c->ssy + sby) * c->stride + mbx * c->ssx + sbx) << 3]);
            }
        }
        
        if (++mbx >= m_blockWidth)
        {
            mbx = 0;
            if (++mby >= m_blockHeight)
                break;
        }
        
        if (rstinterval && !(--rstcount))
        {
            ByteAlign();
            i = GetBits(pos, 16);
            
            if (((i & 0xFFF8) != 0xFFD0) || ((i & 7) != nextrst))
                throw std::exception();
            
            nextrst = (nextrst + 1) & 7;
            rstcount = rstinterval;
            
            for (i = 0; i < 3; i++)
                comp[i].dcpred = 0;
        }
    }
}

#define CF4A (-9)
#define CF4B (111)
#define CF4C (29)
#define CF4D (-3)
#define CF3A (28)
#define CF3B (109)
#define CF3C (-9)
#define CF3X (104)
#define CF3Y (27)
#define CF3Z (-3)
#define CF2A (139)
#define CF2B (-11)
#define CF(x) Clip(((x) + 64) >> 7)

void ImageJPG::UpsampleH(Component* c)
{
    const int xmax = c->width - 3;
    unsigned char *out, *lin, *lout;
    int x, y;
    
    out = (unsigned char*)malloc((c->width * c->height) << 1);
    if (!out)
        throw std::exception();
    
    lin = c->pixels;
    lout = out;
    for (y = c->height; y; y--)
    {
        lout[0] = CF(CF2A * lin[0] + CF2B * lin[1]);
        lout[1] = CF(CF3X * lin[0] + CF3Y * lin[1] + CF3Z * lin[2]);
        lout[2] = CF(CF3A * lin[0] + CF3B * lin[1] + CF3C * lin[2]);
        for (x = 0; x < xmax; x++)
        {
            lout[(x << 1) + 3] = CF(CF4A * lin[x] + CF4B * lin[x + 1] + CF4C * lin[x + 2] + CF4D * lin[x + 3]);
            lout[(x << 1) + 4] = CF(CF4D * lin[x] + CF4C * lin[x + 1] + CF4B * lin[x + 2] + CF4A * lin[x + 3]);
        }
        lin += c->stride;
        lout += c->width << 1;
        lout[-3] = CF(CF3A * lin[-1] + CF3B * lin[-2] + CF3C * lin[-3]);
        lout[-2] = CF(CF3X * lin[-1] + CF3Y * lin[-2] + CF3Z * lin[-3]);
        lout[-1] = CF(CF2A * lin[-1] + CF2B * lin[-2]);
    }
    c->width <<= 1;
    c->stride = c->width;
    free(c->pixels);
    c->pixels = out;
}

void ImageJPG::UpsampleV(Component* c)
{
    const int w = c->width, s1 = c->stride, s2 = s1 + s1;
    unsigned char *out, *cin, *cout;
    int x, y;
    
    out = (unsigned char*)malloc((c->width * c->height) << 1);
    if (!out)
        throw std::exception();
    
    for (x = 0; x < w; x++)
    {
        cin = &c->pixels[x];
        cout = &out[x];
        *cout = CF(CF2A * cin[0] + CF2B * cin[s1]);  cout += w;
        *cout = CF(CF3X * cin[0] + CF3Y * cin[s1] + CF3Z * cin[s2]);  cout += w;
        *cout = CF(CF3A * cin[0] + CF3B * cin[s1] + CF3C * cin[s2]);  cout += w;
        cin += s1;
        for (y = c->height - 3; y; y--)
        {
            *cout = CF(CF4A * cin[-s1] + CF4B * cin[0] + CF4C * cin[s1] + CF4D * cin[s2]);  cout += w;
            *cout = CF(CF4D * cin[-s1] + CF4C * cin[0] + CF4B * cin[s1] + CF4A * cin[s2]);  cout += w;
            cin += s1;
        }
        cin += s1;
        *cout = CF(CF3A * cin[0] + CF3B * cin[-s1] + CF3C * cin[-s2]);  cout += w;
        *cout = CF(CF3X * cin[0] + CF3Y * cin[-s1] + CF3Z * cin[-s2]);  cout += w;
        *cout = CF(CF2A * cin[0] + CF2B * cin[-s1]);
    }
    c->height <<= 1;
    c->stride = c->width;
    free(c->pixels);
    c->pixels = out;
}

void ImageJPG::Convert()
{
    int i;
    Component* c;
    
    int ncomp = m_bpp / 8;
    for (i = 0, c = comp; i < ncomp; i++, c++)
    {
        while ((c->width < m_width) || (c->height < m_height))
        {
            if (c->width < m_width)
                UpsampleH(c);

            if (c->height < m_height)
                UpsampleV(c);
        }
        
        if ((c->width < m_width) || (c->height < m_height))
            throw std::exception();
    }
    
    if (ncomp == 3)
    {
        // convert to RGB
        int x, yy;
        unsigned char *prgb = rgb;
        const unsigned char *py  = comp[0].pixels;
        const unsigned char *pcb = comp[1].pixels;
        const unsigned char *pcr = comp[2].pixels;
        
        for (yy = m_height; yy; yy--)
        {
            for (x = 0; x < m_width; x++)
            {
                int y = py[x] << 8;
                int cb = pcb[x] - 128;
                int cr = pcr[x] - 128;
                *prgb++ = Clip((y            + 359 * cr + 128) >> 8);
                *prgb++ = Clip((y -  88 * cb - 183 * cr + 128) >> 8);
                *prgb++ = Clip((y + 454 * cb            + 128) >> 8);
            }
            py += comp[0].stride;
            pcb += comp[1].stride;
            pcr += comp[2].stride;
        }
    }
    else if (comp[0].width != comp[0].stride)
    {
        // grayscale -> only remove stride
        unsigned char *pin = &comp[0].pixels[comp[0].stride];
        unsigned char *pout = &comp[0].pixels[comp[0].width];
        int y;
        for (y = comp[0].height - 1; y; y--)
        {
            memcpy(pout, pin, comp[0].width);
            pin += comp[0].stride;
            pout += comp[0].width;
        }
        comp[0].stride = comp[0].width;
    }
}

bool ImageJPG::Decode(unsigned char* fileData, unsigned long fileSize)
{
    try
    {
        unsigned char* pos = fileData;
        
        buf = 0;
        bufbits = 0;
        
        if ((pos[0] ^ 0xFF) | (pos[1] ^ 0xD8))
            throw std::exception();
        
        pos += 2;
        
        bool done = false;
        while (!done)
        {
            if (pos[0] != 0xFF)
                throw std::exception();
            
            pos += 2;
            
            switch (pos[-1])
            {
                case 0xFE: SkipMarker(pos); break;
                case 0xC0: DecodeSOF(pos);  break;
                case 0xC4: DecodeDHT(pos);  break;
                case 0xDB: DecodeDQT(pos);  break;
                case 0xDD: DecodeDRI(pos);  break;
                    
                case 0xDA: DecodeScan(pos);
                    done = true;
                    break;
                
                default:
                    if ((pos[-1] & 0xF0) == 0xE0)
                        SkipMarker(pos);
                    else
                        throw std::exception();
            }
        }
        
        Convert();
        
        m_pixels = new unsigned char[m_imageSize];
        if (!m_pixels)
            return false;
        memcpy(m_pixels, m_bpp == 8 ? comp[0].pixels : rgb, m_imageSize);
        FlipVertical();
        
        for (int i = 0; i < 3; i++)
        {
            if (comp[i].pixels)
            {
                free(comp[i].pixels);
                comp[i].pixels = NULL;
            }
        }
        
        if (rgb)
        {
            free(rgb);
            rgb = NULL;
        }
        
        return true;
    }
    catch (std::exception&)
    {
        return false;
    }
}
