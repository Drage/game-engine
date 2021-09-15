
#ifndef COLOR_H
#define COLOR_H

#include "Vector3.h"
#include "Vector4.h"
#include <SDL2/SDL_pixels.h>

namespace DrageEngine
{
    class Color
    {
        friend std::istream& operator>>(std::istream &stream, Color &colour);
        friend std::ostream& operator<<(std::ostream &stream, const Color &colour);
        
    public:
        float r;
        float g;
        float b;
        float a;
        
        static const Color WHITE;
        static const Color BLACK;
        
        Color();
        Color(float r, float g, float b, float a = 1.0f);
        Color(float luminance);
        Color(const Vector3 &vec3);
        Color(const Vector4 &vec4);
        
        static Color White();
        static Color Black();
        
        Color operator+ (const Color &other) const;
        void operator+= (const Color &other);
        
        Color operator- (const Color &other) const;
        void operator-= (const Color &other);
        
        void operator= (const Color &other);
        
        bool operator== (const Color &other);
        bool operator!= (const Color &other);
        
        Color operator* (float scalar) const;
        
        float& operator[] (int i);
        const float& operator[] (int i) const;
        const float* ToArray() const;
        
        Vector3 ToVector3() const;
        Vector4 ToVector4() const;
        float ToGreyscale() const;
        
        SDL_Color ToSDLColor() const;
        
        std::string ToString() const;
        static Color Parse(const std::string &str);
    };
}

#endif
