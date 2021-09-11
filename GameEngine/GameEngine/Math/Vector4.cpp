
#include <cmath>
#include <sstream>
#include "Vector4.h"
#include "MathUtils.h"
#include "Debug.h"

using namespace DrageEngine;

const Vector4 Vector4::ZERO = Vector4(0, 0, 0, 0);
const Vector4 Vector4::ONE = Vector4(1, 1, 1, 1);

Vector4::Vector4()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

Vector4::Vector4(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector4::Vector4(float xyzw)
{
    x = xyzw;
    y = xyzw;
    z = xyzw;
    w = xyzw;
}

Vector4::Vector4(const Vector4 &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
}

Vector4::Vector4(const Vector3 &vec3, float w)
{
    x = vec3.x;
    y = vec3.y;
    z = vec3.z;
    this->w = w;
}

Vector3 Vector4::xyz() const
{
    return Vector3(x, y, z);
}

Vector4 Vector4::operator+ (const Vector4 &other) const
{
    return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4 Vector4::operator- (const Vector4 &other) const
{
    return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4 Vector4::operator- () const
{
    return Vector4(-x, -y, -z, -w);
}

Vector4 Vector4::operator* (float operand) const
{
    return Vector4(x * operand, y * operand, z * operand, w * operand);
}

Vector4 Vector4::operator/ (float operand) const
{
    return Vector4(x / operand, y / operand, z / operand, w / operand);
}

void Vector4::operator+= (const Vector4 &other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
}

void Vector4::operator-= (const Vector4 &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
}

void Vector4::operator*= (float operand)
{
    x *= operand;
    y *= operand;
    z *= operand;
    w *= operand;
}

void Vector4::operator/= (float operand)
{
    x /= operand;
    y /= operand;
    z /= operand;
    w /= operand;
}

bool Vector4::operator== (const Vector4 &other)
{
    return (abs(x - other.x) < EPSILON && abs(y - other.y) < EPSILON && abs(z - other.z) < EPSILON && abs(w - other.w) < EPSILON);
}

bool Vector4::operator!= (const Vector4 &other)
{
    return (abs(x - other.x) > EPSILON || abs(y - other.y) > EPSILON || abs(z - other.z) > EPSILON || abs(w - other.w) > EPSILON);
}

void Vector4::operator= (const Vector4 &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
}

float Vector4::Magnitude() const
{
    return sqrt((x * x) + (y * y) + (z * z) + (w * w));
}

float Vector4::MagnitudeSqr() const
{
    return (x * x) + (y * y) + (z * z) + (w * w);
}

Vector4 Vector4::Normalize()
{
    float magnitude = Magnitude();
    
    if (magnitude)
    {
        x /= magnitude;
        y /= magnitude;
        z /= magnitude;
        w /= magnitude;
    }
    
    return *this;
}

Vector4 Vector4::Normalized() const
{
    float magnitude = Magnitude();
    Vector4 normalized(*this);
    
    if (magnitude)
        normalized /= magnitude;
    
    return normalized;
}

float Vector4::Distance(const Vector4 &a, const Vector4 &b)
{
    return (a - b).Magnitude();
}

float Vector4::Dot(const Vector4 &lhs, const Vector4 &rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

Vector4 Vector4::Lerp(const Vector4 &from, const Vector4 &to, float percent)
{
    return from + (to - from) * percent;
}

Vector4 Vector4::MoveTowards(const Vector4 &current, const Vector4 &target, float maxDistanceDelta)
{
    Vector4 delta = target - current;
    float distance = delta.Magnitude();
    return current + delta.Normalized() * (distance > maxDistanceDelta ? maxDistanceDelta : distance);
}

Vector4 Vector4::Min(const Vector4 &a, const Vector4 &b)
{
    return Vector4(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y, a.z < b.z ? a.z : b.z, a.w < b.w ? a.w : b.w);
}

Vector4 Vector4::Max(const Vector4 &a, const Vector4 &b)
{
    return Vector4(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y, a.z > b.z ? a.z : b.z, a.w > b.w ? a.w : b.w);
}

float& Vector4::operator[] (int i)
{
    if (i >= 0 && i < 4)
        return *(&x + i);
    else
    {
        ERROR("Attempting to access invalid vector index");
        return x;
    }
}

const float& Vector4::operator[] (int i) const
{
    if (i >= 0 && i < 4)
        return *(&x + i);
    else
    {
        ERROR("Attempting to access invalid vector index");
        return x;
    }
}

const float* Vector4::ToArray() const
{
    return &x;
}

Vector4 Vector4::FromArray(float *values)
{
    return Vector4(values[0], values[1], values[2], values[3]);
}

std::string Vector4::ToString() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

Vector4 Vector4::Parse(const std::string &str)
{
    Vector4 result;
    std::stringstream(str) >> result;
    return result;
}

std::istream& DrageEngine::operator>> (std::istream &stream, Vector4 &vec)
{
    char comma;
    stream >> vec.x >> comma >> vec.y >> comma >> vec.z >> comma >> vec.w;
    return stream;
}

std::ostream& DrageEngine::operator<< (std::ostream &stream, const Vector4 &vector)
{
    stream << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w;
    return stream;
}
