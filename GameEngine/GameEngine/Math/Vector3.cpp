
#include <cmath>
#include <sstream>
#include "Vector3.h"
#include "MathUtils.h"
#include "Debug.h"

using namespace DrageEngine;

const Vector3 Vector3::RIGHT = Vector3(1, 0, 0);
const Vector3 Vector3::LEFT = Vector3(-1, 0, 0);
const Vector3 Vector3::UP = Vector3(0, 1, 0);
const Vector3 Vector3::DOWN = Vector3(0, -1, 0);
const Vector3 Vector3::FORWARD = Vector3(0, 0, 1);
const Vector3 Vector3::BACK = Vector3(0, 0, -1);
const Vector3 Vector3::ZERO = Vector3(0, 0, 0);
const Vector3 Vector3::ONE = Vector3(1, 1, 1);

Vector3::Vector3()
{
    x = 0;
    y = 0;
    z = 0;
}

Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3::Vector3(float xyz)
{
    x = xyz;
    y = xyz;
    z = xyz;
}

Vector3::Vector3(const Vector3 &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}

Vector3::Vector3(const Vector2 &vec2, float z)
{
    x = vec2.x;
    y = vec2.y;
    this->z = z;
}

Vector3 Vector3::operator+ (const Vector3 &other) const
{
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator- (const Vector3 &other) const
{
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator- () const
{
    return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator* (float operand) const
{
    return Vector3(x * operand, y * operand, z * operand);
}

Vector3 Vector3::operator/ (float operand) const
{
    return Vector3(x / operand, y / operand, z / operand);
}

void Vector3::operator+= (const Vector3 &other)
{
    x += other.x;
    y += other.y;
    z += other.z;
}

void Vector3::operator-= (const Vector3 &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
}

void Vector3::operator*= (float operand)
{
    x *= operand;
    y *= operand;
    z *= operand;
}

void Vector3::operator/= (float operand)
{
    x /= operand;
    y /= operand;
    z /= operand;
}

void Vector3::operator= (const Vector3 &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}

bool Vector3::operator== (const Vector3 &other)
{
    return (abs(x - other.x) < EPSILON && abs(y - other.y) < EPSILON && abs(z - other.z) < EPSILON);
}

bool Vector3::operator!= (const Vector3 &other)
{
    return (abs(x - other.x) > EPSILON || abs(y - other.y) > EPSILON || abs(z - other.z) > EPSILON);
}

bool Vector3::operator< (const Vector3 &other) const
{
    return Magnitude() < other.Magnitude();
}

bool Vector3::operator> (const Vector3 &other) const
{
    return Magnitude() > other.Magnitude();
}

float Vector3::Magnitude() const
{
    return sqrt((x * x) + (y * y) + (z * z));
}

float Vector3::MagnitudeSqr() const
{
    return (x * x) + (y * y) + (z * z);
}

Vector3 Vector3::Normalize()
{
    float magnitude = Magnitude();
    
    if (magnitude)
    {
        x /= magnitude;
        y /= magnitude;
        z /= magnitude;
    }
    
    return *this;
}

Vector3 Vector3::Normalized() const
{
    float magnitude = Magnitude();
    Vector3 normalized(*this);
    
    if (magnitude)
        normalized /= magnitude;
    
    return normalized;
}

Vector3 Vector3::Scale(const Vector3 &a, const Vector3 &b)
{
    return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

float Vector3::Distance(const Vector3 &a, const Vector3 &b)
{
    return (a - b).Magnitude();
}

float Vector3::Dot(const Vector3 &lhs, const Vector3 &rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vector3 Vector3::Cross(const Vector3 &lhs, const Vector3 &rhs)
{
    return Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
}

float Vector3::Angle(const Vector3 &a, const Vector3 &b)
{
    return acos(Dot(a, b) / (a.Magnitude() * b.Magnitude()));
}

float Vector3::SignedAngle(const Vector3 &a, const Vector3 &b, const Vector3 &axis)
{
    return atan2(Dot(Cross(b, a), axis), Dot(a, b));
}

Vector3 Vector3::Reflect(const Vector3 &direction, const Vector3 &normal)
{
    float dot = Dot(direction, normal);
    return direction + normal * 2 * -dot;
}

Vector3 Vector3::Project(const Vector3 &vector, const Vector3 &normal)
{
    return (normal / normal.MagnitudeSqr()) * Dot(vector, normal);
}

Vector3 Vector3::Lerp(const Vector3 &from, const Vector3 &to, float percent)
{
    return from + (to - from) * percent;
}

Vector3 Vector3::MoveTowards(const Vector3 &current, const Vector3 &target, float maxDistanceDelta)
{
    Vector3 delta = target - current;
    float distance = delta.Magnitude();
    return current + delta.Normalized() * (distance > maxDistanceDelta ? maxDistanceDelta : distance);
}

Vector3 Vector3::Min(const Vector3 &a, const Vector3 &b)
{
    return Vector3(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y, a.z < b.z ? a.z : b.z);
}

Vector3 Vector3::Max(const Vector3 &a, const Vector3 &b)
{
    return Vector3(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y, a.z > b.z ? a.z : b.z);
}

float& Vector3::operator[] (int i)
{
    if (i >= 0 && i < 3)
        return *(&x + i);
    else
    {
        ERROR("Attempting to access invalid vector index");
        return x;
    }
}

const float& Vector3::operator[] (int i) const
{
    if (i >= 0 && i < 3)
        return *(&x + i);
    else
    {
        ERROR("Attempting to access invalid vector index");
        return x;
    }
}

const float* Vector3::ToArray() const
{
    return &x;
}

Vector3 Vector3::FromArray(float *values)
{
    return Vector3(values[0], values[1], values[2]);
}

std::string Vector3::ToString() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

Vector3 Vector3::Parse(const std::string &str)
{
    Vector3 result;
    std::stringstream(str) >> result;
    return result;
}

std::istream& DrageEngine::operator>> (std::istream &stream, Vector3 &vector)
{
    char comma;
    stream >> vector.x >> comma >> vector.y >> comma >> vector.z;
    return stream;
}

std::ostream& DrageEngine::operator<< (std::ostream &stream, const Vector3 &vector)
{
    stream << vector.x << ", " << vector.y << ", " << vector.z;
    return stream;
}
