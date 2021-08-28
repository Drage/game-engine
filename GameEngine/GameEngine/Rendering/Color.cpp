
#include <sstream>
#include <iomanip>
#include <cmath>
#include "Color.h"
#include "Debug.h"
#include "MathUtils.h"

using namespace DrageEngine;

const Color Color::WHITE = Color(1, 1, 1, 1);
const Color Color::BLACK = Color(0, 0, 0, 1);

Color::Color()
{
    this->r = 0;
    this->g = 0;
    this->b = 0;
    this->a = 1;
}

Color::Color(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

Color::Color(float l)
{
    this->r = l;
    this->g = l;
    this->b = l;
    this->a = 1;
}

Color::Color(const Vector3 &vec3)
{
    r = vec3.x;
    g = vec3.y;
    b = vec3.z;
    a = 1;
}

Color::Color(const Vector4 &vec4)
{
    r = vec4.w;
    g = vec4.x;
    b = vec4.y;
    a = vec4.z;
}

Color Color::White()
{
    return Color(1, 1, 1, 1);
}

Color Color::Black()
{
    return Color(0, 0, 0, 1);
}

Color Color::operator+ (const Color &other) const
{
    float rx, gx, bx, ax;
    
    ax = 1 - (1 - a) * (1 - other.a);
    rx = r * a / ax + other.r * other.a * (1 - a) / ax;
    gx = g * a / ax + other.g * other.a * (1 - a) / ax;
    bx = b * a / ax + other.b * other.a * (1 - a) / ax;
    
    return Color(rx, gx, bx, ax);
}

void Color::operator+= (const Color &other)
{
    *this = (*this) + other;
}

Color Color::operator- (const Color &other) const
{
    return Color(r - other.r, g - other.g, b - other.b, a - other.a);
}

void Color::operator-= (const Color &other)
{
    r -= other.r;
    g -= other.g;
    b -= other.b;
    a -= other.a;
}

void Color::operator= (const Color &other)
{
    r = other.r;
    g = other.g;
    b = other.b;
    a = other.a;
}

bool Color::operator== (const Color &other)
{
    return (abs(r - other.r) < EPSILON && abs(g - other.g) < EPSILON && abs(b - other.b) < EPSILON && abs(a - other.a) < EPSILON);
}

bool Color::operator!= (const Color &other)
{
    return (abs(r - other.r) > EPSILON || abs(g - other.g) > EPSILON || abs(b - other.b) > EPSILON || abs(a - other.a) > EPSILON);
}

Color Color::operator* (float scalar) const
{
    return Color(Clamp(r * scalar, 0.0f, 1.0f), Clamp(g * scalar, 0.0f, 1.0f),
                 Clamp(b * scalar, 0.0f, 1.0f), Clamp(a * scalar, 0.0f, 1.0f));
}

float& Color::operator[] (int i)
{
    if (i >= 0 && i < 4)
        return *(&r + i);
    else
    {
        ERROR("Attempting to access invalid color index");
        return r;
    }
}

const float& Color::operator[] (int i) const
{
    if (i >= 0 && i < 4)
        return *(&r + i);
    else
    {
        ERROR("Attempting to access invalid color index");
        return r;
    }
}

const float* Color::ToArray() const
{
    return &r;
}

Vector3 Color::ToVector3()
{
    return Vector3(r, g, b);
}

Vector4 Color::ToVector4()
{
    return Vector4(r, g, b, a);
}

float Color::ToGreyscale()
{
    return (r + g + b) / 3;
}

std::string Color::ToString() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

Color Color::Parse(const std::string &str)
{
    Color result;
    std::stringstream(str) >> result;
    return result;
}

std::istream& DrageEngine::operator>> (std::istream &stream, Color &color)
{
    std::string hex;
    stream >> hex;
    
    std::string redStr = hex.substr(1, 2);
    std::string greenStr = hex.substr(3, 2);
    std::string blueStr = hex.substr(5, 2);
    
    unsigned int red, green, blue;
    sscanf(redStr.c_str(), "%x", &red);
    sscanf(greenStr.c_str(), "%x", &green);
    sscanf(blueStr.c_str(), "%x", &blue);
    
    color.r = red / 255.0f;
    color.g = green / 255.0f;
    color.b = blue / 255.0f;
    color.a = 1;
    
    return stream;
}

std::ostream& DrageEngine::operator<< (std::ostream &stream, const Color &color)
{
    int red = (int)(color.r * 255);
    int green = (int)(color.g * 255);
    int blue = (int)(color.b * 255);
    
    std::stringstream ss;
    ss << std::uppercase << std::setw(2) << std::setfill('0') << std::hex;
    ss << red << green << blue;
    
    stream << "#" + ss.str();
    return stream;
}
