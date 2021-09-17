
#include <cmath>
#include <sstream>
#include "Matrix4x4.h"
#include "Matrix3x3.h"
#include "MathUtils.h"
#include "Debug.h"

using namespace DrageEngine;

const Matrix4x4 Matrix4x4::IDENTITY = Matrix4x4();

Matrix4x4::Matrix4x4()
{
    SetIdentity();
}

Matrix4x4::Matrix4x4(const Matrix3x3 &other)
{
    values[0] = other[0];
    values[1] = other[1];
    values[2] = other[2];
    values[3] = 0.0f;
    values[4] = other[3];
    values[5] = other[4];
    values[6] = other[5];
    values[7] = 0.0f;
    values[8] = other[6];
    values[9] = other[7];
    values[10] = other[8];
    values[11] = 0.0f;
    values[12] = 0.0f;
    values[13] = 0.0f;
    values[14] = 0.0f;
    values[15] = 1.0f;
}

void Matrix4x4::SetIdentity()
{
    // Set all to 0
    for (int i = 0; i < 16; i++)
        values[i] = 0.0f;
    
    // Fill diagonal with 1s
    values[0] = values[5] = values[10] = values[15] = 1.0f;
}

Matrix4x4 Matrix4x4::operator* (const Matrix4x4 &other) const
{
    Matrix4x4 result;
    
    // First Column
    result[0 ] = values[0]*other[0 ] + values[4]*other[1 ] + values[8 ]*other[2 ] + values[12]*other[3 ];
    result[1 ] = values[1]*other[0 ] + values[5]*other[1 ] + values[9 ]*other[2 ] + values[13]*other[3 ];
    result[2 ] = values[2]*other[0 ] + values[6]*other[1 ] + values[10]*other[2 ] + values[14]*other[3 ];
    result[3 ] = values[3]*other[0 ] + values[7]*other[1 ] + values[11]*other[2 ] + values[15]*other[3 ];
    
    // Second Column
    result[4 ] = values[0]*other[4 ] + values[4]*other[5 ] + values[8 ]*other[6 ] + values[12]*other[7 ];
    result[5 ] = values[1]*other[4 ] + values[5]*other[5 ] + values[9 ]*other[6 ] + values[13]*other[7 ];
    result[6 ] = values[2]*other[4 ] + values[6]*other[5 ] + values[10]*other[6 ] + values[14]*other[7 ];
    result[7 ] = values[3]*other[4 ] + values[7]*other[5 ] + values[11]*other[6 ] + values[15]*other[7 ];
    
    // Third Column
    result[8 ] = values[0]*other[8 ] + values[4]*other[9 ] + values[8 ]*other[10] + values[12]*other[11];
    result[9 ] = values[1]*other[8 ] + values[5]*other[9 ] + values[9 ]*other[10] + values[13]*other[11];
    result[10] = values[2]*other[8 ] + values[6]*other[9 ] + values[10]*other[10] + values[14]*other[11];
    result[11] = values[3]*other[8 ] + values[7]*other[9 ] + values[11]*other[10] + values[15]*other[11];
    
    // Fourth Column
    result[12] = values[0]*other[12] + values[4]*other[13] + values[8 ]*other[14] + values[12]*other[15];
    result[13] = values[1]*other[12] + values[5]*other[13] + values[9 ]*other[14] + values[13]*other[15];
    result[14] = values[2]*other[12] + values[6]*other[13] + values[10]*other[14] + values[14]*other[15];
    result[15] = values[3]*other[12] + values[7]*other[13] + values[11]*other[14] + values[15]*other[15];
    
    return result;
}

Matrix4x4 Matrix4x4::operator+ (const Matrix4x4 &other) const
{
    Matrix4x4 result;
    
    for (int i = 0; i < 16; i++)
        result[i] = values[i] + other[i];
        
        return result;
}

Matrix4x4 Matrix4x4::operator- (const Matrix4x4 &other) const
{
    Matrix4x4 result;
    
    for (int i = 0; i < 16; i++)
        result[i] = values[i] - other[i];
        
        return result;
}

Vector4 Matrix4x4::operator* (const Vector4 &point) const
{
    Vector4 result;
    
    result[0] = values[0]*point[0] + values[4]*point[1] + values[8 ]*point[2] + values[12]*point[3];
    result[1] = values[1]*point[0] + values[5]*point[1] + values[9 ]*point[2] + values[13]*point[3];
    result[2] = values[2]*point[0] + values[6]*point[1] + values[10]*point[2] + values[14]*point[3];
    result[3] = values[3]*point[0] + values[7]*point[1] + values[11]*point[2] + values[15]*point[3];
    
    return result;
}

Vector3 Matrix4x4::operator* (const Vector3 &point) const
{
    Vector3 result;
    
    result[0] = values[0]*point[0] + values[4]*point[1] + values[8 ]*point[2];
    result[1] = values[1]*point[0] + values[5]*point[1] + values[9 ]*point[2];
    result[2] = values[2]*point[0] + values[6]*point[1] + values[10]*point[2];
    
    return result;
}

void Matrix4x4::operator*= (const Matrix4x4 &other)
{
    *this = (*this) * other;
}

void Matrix4x4::operator+= (const Matrix4x4 &other)
{
    *this = (*this) + other;
}

void Matrix4x4::operator-= (const Matrix4x4 &other)
{
    *this = (*this) - other;
}

void Matrix4x4::SetTranslation(const Vector3 &translation)
{
    SetIdentity();
    values[12] = translation.x;
    values[13] = translation.y;
    values[14] = translation.z;
}

void Matrix4x4::SetScale(const Vector3 &scale)
{
    SetIdentity();
    values[0] = scale.x;
    values[5] = scale.y;
    values[10] = scale.z;
}

void Matrix4x4::SetRotation(const Vector3 &axis, float angle)
{
    float radAngle = DegToRad(angle);
    float cosine = cos(radAngle);
    float sine = sin(radAngle);
    float x2 = axis.x*axis.x;
    float y2 = axis.y*axis.y;
    float z2 = axis.z*axis.z;
    
    values[0] = x2 + (y2 + z2) * cosine;
    values[4] = axis.x * axis.y * (1 - cosine) - axis.z * sine;
    values[8] = axis.x * axis.z * (1 - cosine) + axis.y * sine;
    values[12]= 0.0f;
    
    values[1] = axis.x * axis.y * (1 - cosine) + axis.z * sine;
    values[5] = y2 + (x2 + z2) * cosine;
    values[9] = axis.y * axis.z * (1 - cosine) - axis.x * sine;
    values[13]= 0.0f;
    
    values[2] = axis.x * axis.z * (1 - cosine) - axis.y * sine;
    values[6] = axis.y * axis.z * (1 - cosine) + axis.x * sine;
    values[10]= z2 + (x2 + y2) * cosine;
    values[14]= 0.0f;
    
    values[3] = 0.0f;
    values[7] = 0.0f;
    values[11]= 0.0f;
    values[15]= 1.0f;
}

void Matrix4x4::SetRotation(const Vector3 &rotation)
{
    Matrix4x4 X, Y, Z;
    float x, y, z;
    
    x = DegToRad(rotation.x);
    y = DegToRad(rotation.y);
    z = DegToRad(rotation.z);
    
    Y.SetRotation(Vector3(0, 1, 0), y);
    X.SetRotation(Vector3(1, 0, 0), x);
    Z.SetRotation(Vector3(0, 0, 1), z);
    
    *this = Y * X * Z;
}

void Matrix4x4::SetRotation(const Vector3 &right, const Vector3 &up, const Vector3 &forward)
{
    values[0] = right.x;      values[1] = right.y;      values[2] = right.z;      values[3] = 0;
    values[4] = up.x;         values[5] = up.y;         values[6] = up.z;         values[7] = 0;
    values[8] = forward.x;    values[9] = forward.y;    values[10] = forward.z;   values[11] = 0;
    values[12] = 0;           values[13] = 0;           values[14] = 0;           values[15] = 1;
}
void Matrix4x4::SetRotation(const Quaternion &quat)
{
    // First column
    values[0] = 1 - 2 * (quat.y * quat.y + quat.z * quat.z);
    values[1] = 2 * (quat.x * quat.y + quat.z * quat.w);
    values[2] = 2 * (quat.x * quat.z - quat.y * quat.w);
    values[3] = 0;
    
    // Second Column
    values[4] = 2 * (quat.x * quat.y - quat.z * quat.w);
    values[5] = 1 - 2 * (quat.x * quat.x + quat.z * quat.z);
    values[6] = 2 * (quat.z * quat.y + quat.x * quat.w);
    values[7] = 0;
    
    // Third Column
    values[8] = 2 * (quat.x * quat.z + quat.y * quat.w);
    values[9] = 2 * (quat.y * quat.z - quat.x * quat.w);
    values[10] = 1 - 2 * (quat.x * quat.x + quat.y * quat.y);
    values[11] = 0;
    
    // Fourth Column
    values[12] = 0;
    values[13] = 0;
    values[14] = 0;
    values[15] = 1;
}

void Matrix4x4::Invert()
{
    *this = Inverse();
}

Matrix4x4 Matrix4x4::Inverse() const
{
    Matrix4x4 inv;
    const float *m = *(&values);

    inv[0] = m[5]  * m[10] * m[15] -
             m[5]  * m[11] * m[14] -
             m[9]  * m[6]  * m[15] +
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] -
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] +
              m[4]  * m[11] * m[14] +
              m[8]  * m[6]  * m[15] -
              m[8]  * m[7]  * m[14] -
              m[12] * m[6]  * m[11] +
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] -
             m[4]  * m[11] * m[13] -
             m[8]  * m[5] * m[15] +
             m[8]  * m[7] * m[13] +
             m[12] * m[5] * m[11] -
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] +
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] -
               m[8]  * m[6] * m[13] -
               m[12] * m[5] * m[10] +
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] +
              m[1]  * m[11] * m[14] +
              m[9]  * m[2] * m[15] -
              m[9]  * m[3] * m[14] -
              m[13] * m[2] * m[11] +
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] -
             m[0]  * m[11] * m[14] -
             m[8]  * m[2] * m[15] +
             m[8]  * m[3] * m[14] +
             m[12] * m[2] * m[11] -
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] +
              m[0]  * m[11] * m[13] +
              m[8]  * m[1] * m[15] -
              m[8]  * m[3] * m[13] -
              m[12] * m[1] * m[11] +
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] -
              m[0]  * m[10] * m[13] -
              m[8]  * m[1] * m[14] +
              m[8]  * m[2] * m[13] +
              m[12] * m[1] * m[10] -
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] -
             m[1]  * m[7] * m[14] -
             m[5]  * m[2] * m[15] +
             m[5]  * m[3] * m[14] +
             m[13] * m[2] * m[7] -
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] +
              m[0]  * m[7] * m[14] +
              m[4]  * m[2] * m[15] -
              m[4]  * m[3] * m[14] -
              m[12] * m[2] * m[7] +
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] -
              m[0]  * m[7] * m[13] -
              m[4]  * m[1] * m[15] +
              m[4]  * m[3] * m[13] +
              m[12] * m[1] * m[7] -
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] +
               m[0]  * m[6] * m[13] +
               m[4]  * m[1] * m[14] -
               m[4]  * m[2] * m[13] -
               m[12] * m[1] * m[6] +
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
              m[1] * m[7] * m[10] +
              m[5] * m[2] * m[11] -
              m[5] * m[3] * m[10] -
              m[9] * m[2] * m[7] +
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
             m[0] * m[7] * m[10] -
             m[4] * m[2] * m[11] +
             m[4] * m[3] * m[10] +
             m[8] * m[2] * m[7] -
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
               m[0] * m[7] * m[9] +
               m[4] * m[1] * m[11] -
               m[4] * m[3] * m[9] -
               m[8] * m[1] * m[7] +
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
              m[0] * m[6] * m[9] -
              m[4] * m[1] * m[10] +
              m[4] * m[2] * m[9] +
              m[8] * m[1] * m[6] -
              m[8] * m[2] * m[5];

    float determinant = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (!determinant)
    {
        ERROR("Cannot calculate matrix inverse - Null determinant");
        return Matrix4x4::IDENTITY;
    }

    determinant = 1.0 / determinant;

    for (int i = 0; i < 16; i++)
        inv[i] *= determinant;

    return inv;
}

void Matrix4x4::Transpose()
{
    *this = this->Transposed();
}

Matrix4x4 Matrix4x4::Transposed() const
{
    Matrix4x4 transpose;
    
    transpose[0 ] = values[0];    transpose[1 ] = values[4];    transpose[2 ] = values[8];    transpose[3 ] = values[12];
    transpose[4 ] = values[1];    transpose[5 ] = values[5];    transpose[6 ] = values[9];    transpose[7 ] = values[13];
    transpose[8 ] = values[2];    transpose[9 ] = values[6];    transpose[10] = values[10];   transpose[7 ] = values[14];
    transpose[12] = values[3];    transpose[13] = values[7];    transpose[14] = values[11];   transpose[15] = values[15];
    
    return transpose;
}

void Matrix4x4::SetProjectionPerspective(float fov, float apectRatio, float nearClip, float farClip)
{
    float size = nearClip * tanf(DegToRad(fov) / 2.0f);
    float left = -size, right = size, bottom = -size / apectRatio, top = size / apectRatio;
    
    values[0] = 2 * nearClip / (right - left);
    values[1] = 0;
    values[2] = 0;
    values[3] = 0;
    
    values[4] = 0;
    values[5] = 2 * nearClip / (top - bottom);
    values[6] = 0;
    values[7] = 0;
    
    values[8] = (right + left) / (right - left);
    values[9] = (top + bottom) / (top - bottom);
    values[10] = -(farClip + nearClip) / (farClip - nearClip);
    values[11] = -1;
    
    values[12] = 0;
    values[13] = 0;
    values[14] = -(2 * farClip * nearClip) / (farClip - nearClip);
    values[15] = 0;
}

void Matrix4x4::SetProjectionOrthographic(int width, int height)
{
    float left, right, top, bottom, far, near;
    left = 0;
    right = (float)width;
    top = (float)height;
    bottom = 0;
    near = -1;
    far = 1;
    
    values[0] = 2.0f / (right - left);
    values[1] = 0;
    values[2] = 0;
    values[3] = 0;
    
    values[4] = 0;
    values[5] = 2.0f / (top - bottom);
    values[6] = 0;
    values[7] = 0;
    
    values[8] = 0;
    values[9] = 0;
    values[10] = -2.0f / (far - near);
    values[11] = 0;
    
    values[12] = -(right + left) / (right - left);
    values[13] = -(top + bottom) / (top - bottom);
    values[14] = -(far + near) / (far - near);
    values[15] = 1;
}

void Matrix4x4::SetView(const Vector3 &position, const Vector3 &direction, const Vector3 &upVector)
{
    Vector3 forward = direction.Normalized();
    Vector3 up = upVector.Normalized();
    Vector3 side = Vector3::Cross(forward, up).Normalized();
    up = Vector3::Cross(side, forward).Normalized();
    
    // First column
    values[0 ] = side[0];
    values[1 ] = up[0];
    values[2 ] = -forward[0];
    values[3 ] = 0.0f;
    
    // Second column
    values[4 ] = side[1];
    values[5 ] = up[1];
    values[6 ] = -forward[1];
    values[7 ] = 0.0f;
    
    // Third column
    values[8 ] = side[2];
    values[9 ] = up[2];
    values[10] = -forward[2];
    values[11] =  0.0f;
    
    // fourth column
    values[12] = 0.0f;
    values[13] = 0.0f;
    values[14] = 0.0f;
    values[15] = 1.0f;
    
    Matrix4x4 translation;
    translation.SetTranslation(-position);
    *this *= translation;
}

Vector3 Matrix4x4::GetTranslation() const
{
    return Vector3(values[12], values[13], values[14]);
}

Vector3 Matrix4x4::GetScale() const
{
    return Vector3(values[0], values[5], values[10]);
}

Quaternion Matrix4x4::GetRotation() const
{
    Quaternion q;
    
    q.w = sqrt(Max<float>(0, 1 + values[0] + values[5] + values[10])) / 2.0f;
    q.x = sqrt(Max<float>(0, 1 + values[0] - values[5] - values[10])) / 2.0f;
    q.y = sqrt(Max<float>(0, 1 - values[0] + values[5] - values[10])) / 2.0f;
    q.z = sqrt(Max<float>(0, 1 - values[0] - values[5] + values[10])) / 2.0f;
    
    q.x *= Sign<float>(q.x * (values[6] - values[9]));
    q.y *= Sign<float>(q.y * (values[8] - values[2]));
    q.z *= Sign<float>(q.z * (values[1] - values[4]));
    
    q.Normalize();
    return q;
}

Vector4 Matrix4x4::GetColumn(int column) const
{
    switch (column)
    {
        case 0: return Vector4(values[0], values[1], values[2], values[3]);
        case 1: return Vector4(values[4], values[5], values[6], values[7]);
        case 2: return Vector4(values[8], values[9], values[10], values[11]);
        case 3: return Vector4(values[12], values[13], values[14], values[15]);
        default:
            ERROR("Undefined matrix column index");
            return Vector4(0);
    }
}

float& Matrix4x4::operator[] (int i)
{
    if (i >= 0 && i < 16)
        return values[i];
    else
    {
        ERROR("Attempting to access invalid matrix index");
        return values[0];
    }
}

const float& Matrix4x4::operator[] (int i) const
{
    if (i >= 0 && i < 16)
        return values[i];
    else
    {
        ERROR("Attempting to access invalid matrix index");
        return values[0];
    }
}

const float* Matrix4x4::ToArray() const
{
    return values;
}

std::string Matrix4x4::ToString() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

Matrix4x4 Matrix4x4::Parse(const std::string &str)
{
    Matrix4x4 result;
    std::stringstream(str) >> result;
    return result;
}

std::istream& DrageEngine::operator>> (std::istream &stream, Matrix4x4 &matrix)
{
    char comma, pipe;
    stream >> matrix[0] >> comma >> matrix[4] >> comma >> matrix[8] >> comma >> matrix[12] >> pipe;
    stream >> matrix[1] >> comma >> matrix[5] >> comma >> matrix[9] >> comma >> matrix[13] >> pipe;
    stream >> matrix[2] >> comma >> matrix[6] >> comma >> matrix[10] >> comma >> matrix[14] >> pipe;
    stream >> matrix[3] >> comma >> matrix[7] >> comma >> matrix[11] >> comma >> matrix[15];
    return stream;
}

std::ostream& DrageEngine::operator<< (std::ostream &stream, const Matrix4x4 &matrix)
{
    stream << matrix[0] << ", " << matrix[4] << ", " << matrix[8] << ", " << matrix[12] << " | ";
    stream << matrix[1] << ", " << matrix[5] << ", " << matrix[9] << ", " << matrix[13] << " | ";
    stream << matrix[2] << ", " << matrix[6] << ", " << matrix[10] << ", " << matrix[14] << " | ";
    stream << matrix[3] << ", " << matrix[7] << ", " << matrix[11] << ", " << matrix[15];
    return stream;
}
