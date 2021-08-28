
#include <cmath>
#include <sstream>
#include "Vector2.h"
#include "MathUtils.h"
#include "Debug.h"

using namespace DrageEngine;

const Vector2 Vector2::RIGHT = Vector2(1, 0);
const Vector2 Vector2::LEFT = Vector2(-1, 0);
const Vector2 Vector2::UP = Vector2(0, 1);
const Vector2 Vector2::DOWN = Vector2(0, -1);
const Vector2 Vector2::ZERO = Vector2(0, 0);
const Vector2 Vector2::ONE = Vector2(1, 1);

Vector2::Vector2()
{
    x = 0;
    y = 0;
}

Vector2::Vector2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2::Vector2(float xy)
{
    x = xy;
    y = xy;
}

Vector2::Vector2(const Vector2 &other)
{
    x = other.x;
    y = other.y;
}

Vector2 Vector2::operator+ (const Vector2 &other) const
{
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator- (const Vector2 &other) const
{
    return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator- () const
{
    return Vector2(-x, -y);
}

Vector2 Vector2::operator* (float operand) const
{
    return Vector2(x * operand, y * operand);
}

Vector2 Vector2::operator/ (float operand) const
{
    return Vector2(x / operand, y / operand);
}

void Vector2::operator+= (const Vector2 &other)
{
    x += other.x;
    y += other.y;
}

void Vector2::operator-= (const Vector2 &other)
{
    x -= other.x;
    y -= other.y;
}

void Vector2::operator*= (float operand)
{
    x *= operand;
    y *= operand;
}

void Vector2::operator/= (float operand)
{
    x /= operand;
    y /= operand;
}

void Vector2::operator= (const Vector2 &other)
{
    x = other.x;
    y = other.y;
}

bool Vector2::operator== (const Vector2 &other)
{
    return (abs(x - other.x) < EPSILON && abs(y - other.y) < EPSILON);
}

bool Vector2::operator!= (const Vector2 &other)
{
    return (abs(x - other.x) > EPSILON || abs(y - other.y) > EPSILON);
}

float Vector2::Magnitude() const
{
    return sqrt((x * x) + (y * y));
}

float Vector2::MagnitudeSqr() const
{
    return (x * x) + (y * y);
}

Vector2 Vector2::Normalize()
{
    float magnitude = Magnitude();
    if (magnitude)
    {
        x /= magnitude;
        y /= magnitude;
    }
    return *this;
}

Vector2 Vector2::Normalized() const
{
    float magnitude = Magnitude();
    Vector2 normalized(*this);
    
    if (magnitude)
        normalized /= magnitude;
    
    return normalized;
}

Vector2 Vector2::Scale(const Vector2 &a, const Vector2 &b)
{
    return Vector2(a.x * b.x, a.y * b.y);
}

float Vector2::Distance(const Vector2 &a, const Vector2 &b)
{
    return (a - b).Magnitude();
}

float Vector2::Dot(const Vector2 &lhs, const Vector2 &rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

Vector2 Vector2::Rotate(float angle)
{
    float cosOfAngle = cos(angle);
    float sinOfAngle = sin(angle);
    
    float rotatedX = x * cosOfAngle - y * sinOfAngle;
    float rotatedY = x * sinOfAngle + y * cosOfAngle;
    
    x = rotatedX;
    x = rotatedY;
    
    return *this;
}

float Vector2::Angle(const Vector2 &a, const Vector2 &b)
{
    return acos(Dot(a, b) / (a.Magnitude() * b.Magnitude()));
}

float Vector2::SignedAngle(const Vector2 &a, const Vector2 &b)
{
    return atan2(b.y, b.x) - atan2(a.y, a.x);
}

Vector2 Vector2::Reflect(const Vector2 &direction, const Vector2 &normal)
{
    float dot = Dot(direction, normal);
    return direction + normal * 2 * -dot;
}

Vector2 Vector2::Perpendicular(const Vector2 &direction)
{
    return Vector2(direction.y, -direction.x);
}

Vector2 Vector2::Lerp(const Vector2 &from, const Vector2 &to, float percent)
{
    return from + (to - from) * percent;
}

Vector2 Vector2::MoveTowards(const Vector2 &current, const Vector2 &target, float maxDistanceDelta)
{
    Vector2 delta = target - current;
    float distance = delta.Magnitude();
    return current + delta.Normalized() * (distance > maxDistanceDelta ? maxDistanceDelta : distance);
}

Vector2 Vector2::Min(const Vector2 &a, const Vector2 &b)
{
    return Vector2(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y);
}

Vector2 Vector2::Max(const Vector2 &a, const Vector2 &b)
{
    return Vector2(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y);
}

float& Vector2::operator[] (int i)
{
    if (i >= 0 && i < 2)
        return *(&x + i);
    else
    {
        ERROR("Attempting to access invalid vector index");
        return x;
    }
}

const float& Vector2::operator[] (int i) const
{
    if (i >= 0 && i < 2)
        return *(&x + i);
    else
    {
        ERROR("Attempting to access invalid vector index");
        return x;
    }
}

const float* Vector2::ToArray() const
{
    return &x;
}

Vector2 Vector2::FromArray(float *values)
{
    return Vector2(values[0], values[1]);
}

std::string Vector2::ToString() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

Vector2 Vector2::Parse(const std::string &str)
{
    Vector2 result;
    std::stringstream(str) >> result;
    return result;
}

std::istream& DrageEngine::operator>> (std::istream &stream, Vector2 &vector)
{
    char comma;
    stream >> vector.x >> comma >> vector.y;
    return stream;
}

std::ostream& DrageEngine::operator<< (std::ostream &stream, const Vector2 &vector)
{
    stream << vector.x << ", " << vector.y;
    return stream;
}
