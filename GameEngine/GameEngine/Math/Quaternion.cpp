
#include <cmath>
#include <sstream>
#include "Quaternion.h"
#include "MathUtils.h"
#include "Matrix4x4.h"

using namespace DrageEngine;

const Quaternion Quaternion::IDENTITY = Quaternion(0, 0, 0, 1);

Quaternion::Quaternion()
{
    SetIdentity();
}

Quaternion::Quaternion(float x, float y,  float z, float w)
{
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
}

Quaternion::Quaternion(const Quaternion &other)
{
    w = other.w;
    x = other.x;
    y = other.y;
    z = other.z;
}

void Quaternion::SetIdentity()
{
    w = 1;
    x = 0;
    y = 0;
    z = 0;
}

Quaternion Quaternion::operator* (const Quaternion &other) const
{
    return Quaternion(w * other.x + x * other.w + y * other.z - z * other.y,
    w * other.y + y * other.w + z * other.x - x * other.z,
    w * other.z + z * other.w + x * other.y - y * other.x,
    w * other.w - x * other.x - y * other.y - z * other.z);
}

void Quaternion::operator*= (const Quaternion &other)
{
    *this = *this * other;
}

Vector3 Quaternion::operator* (const Vector3 &vec) const
{
    Quaternion vecQuat, resQuat;
    vecQuat.x = vec.x;
    vecQuat.y = vec.y;
    vecQuat.z = vec.z;
    vecQuat.w = 0.0f;
    
    resQuat = vecQuat * Conjugate();
    resQuat = *this * resQuat;
    
    return (Vector3(resQuat.x, resQuat.y, resQuat.z));
}

Quaternion Quaternion::operator+ (const Quaternion &other) const
{
    return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
}

Quaternion Quaternion::operator+= (const Quaternion &other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

Quaternion Quaternion::operator- (const Quaternion &other) const
{
    return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
}

Quaternion Quaternion::operator-= (const Quaternion &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

Quaternion Quaternion::operator* (float scale) const
{
    return Quaternion(x * scale, y * scale, z * scale, w * scale);
}

Quaternion Quaternion::operator*= (float scale)
{
    x *= scale;
    y *= scale;
    z *= scale;
    w *= scale;
    return *this;
}

Quaternion Quaternion::operator/ (float scale) const
{
    return Quaternion(x / scale, y / scale, z / scale, w / scale);
}

Quaternion Quaternion::operator/= (float scale)
{
    x /= scale;
    y /= scale;
    z /= scale;
    w /= scale;
    return *this;
}

Quaternion Quaternion::operator- () const
{
    return Quaternion(-x, -y, -z, -w);
}

void Quaternion::operator= (const Quaternion &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
}

float Quaternion::Magnitude() const
{
    return sqrt(w * w + x * x + y * y + z * z);
}

float Quaternion::SqrMagnitude() const
{
    return w * w + x * x + y * y + z * z;
}

void Quaternion::Normalize()
{
    float mag2 = SqrMagnitude();
    
    if (abs(mag2) > EPSILON && abs(mag2 - 1.0f) > EPSILON)
    {
        float mag = Magnitude();
        *this /= mag;
    }
}

Quaternion Quaternion::Normalized() const
{
    float mag2 = SqrMagnitude();
    Quaternion normalized(*this);
    
    if (abs(mag2) > EPSILON && abs(mag2 - 1.0f) > EPSILON)
    {
        float mag = Magnitude();
        normalized /= mag;
    }
    
    return normalized;
}

Quaternion Quaternion::Conjugate() const
{
    return Quaternion(-x, -y, -z, w);
}

void Quaternion::Invert()
{
    *this = Conjugate() / SqrMagnitude();
}

Quaternion Quaternion::Inverse() const
{
    return Conjugate() / SqrMagnitude();
}

float Quaternion::Dot(const Quaternion &lhs, const Quaternion &rhs)
{
    return lhs.w * rhs.w + lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Quaternion Quaternion::FromEuler(float x, float y, float z)
{
    float rx = DegToRad(x) / 2;
    float ry = DegToRad(y) / 2;
    float rz = DegToRad(z) / 2;
    
    float sinX = sin(rx);
    float sinY = sin(ry);
    float sinZ = sin(rz);
    float cosX = cos(rx);
    float cosY = cos(ry);
    float cosZ = cos(rz);
    
    float qw = cosZ * cosY * cosX + sinZ * sinY * sinX;
    float qx = cosZ * cosY * sinX - sinZ * sinY * cosX;
    float qy = cosZ * sinY * cosX + sinZ * cosY * sinX;
    float qz = sinZ * cosY * cosX - cosZ * sinY * sinX;
    
    return Quaternion(qx, qy, qz, qw).Normalized();
}

Quaternion Quaternion::FromEuler(const Vector3 &rotation)
{
    return FromEuler(rotation.x, rotation.y, rotation.z);
}

Quaternion Quaternion::FromAxisAngle(const Vector3 &axis, float angle)
{
    angle *= 0.5f;
    Vector3 vn = axis.Normalized();
    float sinAngle = sin(angle);
    return Quaternion(vn.x * sinAngle, vn.y * sinAngle, vn.z * sinAngle, cos(angle)).Normalized();
}

Quaternion Quaternion::FromLookDirection(const Vector3 &direction)
{
    Vector3 dir = direction.Normalized();
    float dot = Vector3::Dot(Vector3::FORWARD, dir);
    
    if (abs(dot - (-1.0f)) < 0.000001f)
    {
        return Quaternion(0, 1, 0, PI);
    }
    if (abs(dot - (1.0f)) < 0.000001f)
    {
        return Quaternion::IDENTITY;
    }
    
    float rotAngle = (float)acos(dot);
    Vector3 rotAxis = Vector3::Cross(Vector3::FORWARD, dir);
    return FromAxisAngle(rotAxis, rotAngle);
}

Quaternion Quaternion::FromToRotation(const Vector3 &fromDirection, const Vector3 &toDirection)
{
    float angle = Vector3::Angle(fromDirection, toDirection);
    Vector3 axis = Vector3::Cross(fromDirection, toDirection);
    return FromAxisAngle(axis, angle);
}

Vector3 Quaternion::ToEuler() const
{
    Vector3 euler;
    
    float test = x*y + z*w;
    
    if (test > 0.5f - EPSILON)
    {
        euler.y = 2 * atan2(x,w);
        euler.x = PI/2;
        euler.z = 0;
    }
    else if (test < -0.5f + EPSILON)
    {
        euler.y = -2 * atan2(x,w);
        euler.x = -PI/2;
        euler.z = 0;
    }
    else
    {
        euler.y = atan2(2*y*w-2*x*z, 1 - 2*y*y - 2*z*z);
        euler.x = asin(2*x*y + 2*z*w);
        euler.z = atan2(2*x*w-2*y*z, 1 - 2*x*x - 2*z*z);
    }
    
    euler.x = RadToDeg(euler.x);
    euler.y = RadToDeg(euler.y);
    euler.z = RadToDeg(euler.z);
    
    return euler;
}

void Quaternion::ToAxisAngle(Vector3 *axis, float *angle) const
{
    *angle = acos(w);
    float invSinAngle = 1.0f / sin(*angle);
    axis->x = x * invSinAngle;
    axis->y = y * invSinAngle;
    axis->z = z * invSinAngle;
    *angle = RadToDeg((*angle) * 2);
}

Quaternion Quaternion::Slerp(const Quaternion &from, const Quaternion &to, float percent)
{
    Quaternion temp;
    float dot = Dot(from, to);
    
    // If dot < 0 angle is greater than 90 degrees so invert
    if (dot < 0)
    {
        dot = -dot;
        temp = -to;
    }
    else
        temp = to;
    
    if (dot < 0.95f)
    {
        float angle = acosf(dot);
        return (from * sin(angle * (1 - percent)) + temp * sin(angle * percent)) / sin(angle);
    }
    else // Use Lerp for small angle
        return Lerp(from, temp, percent);
}

Quaternion Quaternion::Lerp(const Quaternion &from, const Quaternion &to, float percent)
{
    return (from * (1 - percent) + to * percent).Normalized();
}

std::string Quaternion::ToString() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

Quaternion Quaternion::Parse(const std::string &str)
{
    Quaternion result;
    std::stringstream(str) >> result;
    return result;
}

std::istream& DrageEngine::operator>> (std::istream &stream, Quaternion &quaternion)
{
    char comma;
    float x, y, z;
    stream >> x >> comma >> y >> comma >> z;
    quaternion = Quaternion::FromEuler(x, y, z);
    return stream;
}

std::ostream& DrageEngine::operator<< (std::ostream &stream, const Quaternion &quaternion)
{
    Vector3 euler = quaternion.ToEuler();
    stream << euler.x << ", " << euler.y << ", " << euler.z;
    return stream;
}
