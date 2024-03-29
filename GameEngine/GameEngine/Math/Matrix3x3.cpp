
#include <cmath>
#include <sstream>
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "MathUtils.h"
#include "Debug.h"

using namespace DrageEngine;

const Matrix3x3 Matrix3x3::IDENTITY = Matrix3x3();

Matrix3x3::Matrix3x3()
{
    SetIdentity();
}

Matrix3x3::Matrix3x3(const Matrix4x4 &other)
{
    values[0] = other[0];
    values[1] = other[1];
    values[2] = other[2];
    values[3] = other[4];
    values[4] = other[5];
    values[5] = other[6];
    values[6] = other[8];
    values[7] = other[9];
    values[8] = other[10];
}

void Matrix3x3::SetIdentity()
{
    // Set all to 0
    for (int i = 0; i < 9; i++)
        values[i] = 0.0f;
    
    // Fill diagonal with 1s
    values[0] = values[4] = values[8] = 1.0f;
}

Matrix3x3 Matrix3x3::operator* (const Matrix3x3 &other) const
{
    Matrix3x3 result;
    
    // First Column
    result[0] = values[0]*other[0] + values[3]*other[1] + values[6]*other[2];
    result[1] = values[1]*other[0] + values[4]*other[1] + values[7]*other[2];
    result[2] = values[2]*other[0] + values[5]*other[1] + values[8]*other[2];
    
    // Second Column
    result[3] = values[0]*other[3] + values[3]*other[4] + values[6]*other[5];
    result[4] = values[1]*other[3] + values[4]*other[4] + values[7]*other[5];
    result[5] = values[2]*other[3] + values[5]*other[4] + values[8]*other[5];
    
    // Third Column
    result[6] = values[0]*other[6] + values[3]*other[7] + values[6]*other[8];
    result[7] = values[1]*other[6] + values[4]*other[7] + values[7]*other[8];
    result[8] = values[2]*other[6] + values[5]*other[7] + values[8]*other[8];
    
    return result;
}

Matrix3x3 Matrix3x3::operator+ (const Matrix3x3 &other) const
{
    Matrix3x3 result;
    
    for (int i = 0; i < 9; i++)
        result[i] = values[i] + other[i];
        
        return result;
}

Matrix3x3 Matrix3x3::operator- (const Matrix3x3 &other) const
{
    Matrix3x3 result;
    
    for (int i = 0; i < 9; i++)
        result[i] = values[i] - other[i];
        
        return result;
}

Vector3 Matrix3x3::operator* (const Vector3 &point) const
{
    Vector3 result;
    
    result[0] = values[0]*point[0] + values[3]*point[1] + values[6]*point[2];
    result[1] = values[1]*point[0] + values[4]*point[1] + values[7]*point[2];
    result[2] = values[2]*point[0] + values[5]*point[1] + values[8]*point[2];
    
    return result;
}

void Matrix3x3::operator*= (const Matrix3x3 &other)
{
    *this = (*this) * other;
}

void Matrix3x3::operator+= (const Matrix3x3 &other)
{
    *this = (*this) + other;
}

void Matrix3x3::operator-= (const Matrix3x3 &other)
{
    *this = (*this) - other;
}

void Matrix3x3::SetScale(const Vector3 &scale)
{
    SetIdentity();
    values[0] = scale.x;
    values[4] = scale.y;
    values[8] = scale.z;
}

void Matrix3x3::SetRotation(const Vector3 &axis, float angle)
{
    float radAngle = DegToRad(angle);
    float cosine = cos(radAngle);
    float sine = sin(radAngle);
    float x2 = axis.x*axis.x;
    float y2 = axis.y*axis.y;
    float z2 = axis.z*axis.z;
    
    values[0] = x2 + (y2 + z2) * cosine;
    values[3] = axis.x * axis.y * (1 - cosine) - axis.z * sine;
    values[6] = axis.x * axis.z * (1 - cosine) + axis.y * sine;
    
    values[1] = axis.x * axis.y * (1 - cosine) + axis.z * sine;
    values[4] = y2 + (x2 + z2) * cosine;
    values[7] = axis.y * axis.z * (1 - cosine) - axis.x * sine;
    
    values[2] = axis.x * axis.z * (1 - cosine) - axis.y * sine;
    values[5] = axis.y * axis.z * (1 - cosine) + axis.x * sine;
    values[8]= z2 + (x2 + y2) * cosine;
}

void Matrix3x3::SetRotation(const Vector3 &rotation)
{
    Matrix3x3 X, Y, Z;
    float x, y, z;
    
    x = DegToRad(rotation.x);
    y = DegToRad(rotation.y);
    z = DegToRad(rotation.z);
    
    Y.SetRotation(Vector3(0, 1, 0), y);
    X.SetRotation(Vector3(1, 0, 0), x);
    Z.SetRotation(Vector3(0, 0, 1), z);
    
    *this = Y * X * Z;
}

void Matrix3x3::SetRotation(const Vector3 &right, const Vector3 &up, const Vector3 &forward)
{
    values[0] = right.x;      values[3] = right.y;      values[6] = right.z;
    values[1] = up.x;         values[4] = up.y;         values[7] = up.z;
    values[2] = forward.x;    values[5] = forward.y;    values[8] = forward.z;
}

void Matrix3x3::SkewSymetric(const Vector3 &skew)
{
    values[0] = 0;            values[3] = -skew.z;      values[6] = skew.y;
    values[1] = skew.z;       values[4] = 0;            values[7] = -skew.x;
    values[2] = -skew.y;      values[5] =  skew.x;      values[8] = 0;
}

void Matrix3x3::Invert()
{
    *this = Inverse();
}

Matrix3x3 Matrix3x3::Inverse() const
{
    Matrix3x3 adjugate, inverse;
    float determinant;
    
    // Calculate determinant
    determinant = values[0] * values[4] * values[8] +
    values[1] * values[5] * values[6] +
    values[2] * values[3] * values[7] -
    values[2] * values[4] * values[6] -
    values[1] * values[3] * values[8] -
    values[0] * values[5] * values[7];
    
    // Can't find inverse if determinant is 0
    if (!determinant)
    {
        ERROR("Cannot calculate matrix inverse - Null determinant");
        return Matrix3x3();
    }
    
    // Calculate adjugate matrix
    adjugate[0] = values[4] * values[8] - values[5] * values[7];
    adjugate[1] = values[3] * values[8] - values[5] * values[6];
    adjugate[2] = values[3] * values[7] - values[4] * values[6];
    
    adjugate[3] = values[1] * values[8] - values[2] * values[7];
    adjugate[4] = values[0] * values[8] - values[2] * values[6];
    adjugate[5] = values[0] * values[7] - values[2] * values[6];
    
    adjugate[6] = values[1] * values[5] - values[2] * values[4];
    adjugate[7] = values[0] * values[5] - values[2] * values[3];
    adjugate[8] = values[0] * values[4] - values[1] * values[3];
    
    // Flip sign of odd numbered indexes
    for (int i = 0; i < 9; i++)
    {
        if (i % 2 != 0)
            adjugate[i] = -adjugate[i];
    }
    
    for (int i = 0; i < 9; i++)
        inverse[i] = adjugate[i] / determinant;
    
    return inverse;
}

void Matrix3x3::Orthonormalize()
{
    *this = Orthonormalized();
}

Matrix3x3 Matrix3x3::Orthonormalized() const
{
    Matrix3x3 ortho;
    Vector3 x, y, z;
    
    x = Vector3(values[0], values[1], values[2]);
    y = Vector3(values[3], values[4], values[4]);
    
    x.Normalize();
    z = Vector3::Cross(x, y).Normalize();
    y = Vector3::Cross(z, x).Normalize();
    
    ortho[0] = x.x;     ortho[3] = y.x;     ortho[6] = z.x;
    ortho[1] = x.y;     ortho[4] = y.y;     ortho[7] = z.y;
    ortho[2] = x.z;     ortho[5] = y.z;     ortho[8] = z.z;
    
    return ortho;
}

void Matrix3x3::Transpose()
{
    *this = Transposed();
}

Matrix3x3 Matrix3x3::Transposed() const
{
    Matrix3x3 transposed;
    
    transposed[0] = values[0];    transposed[1] = values[3];    transposed[2] = values[6];
    transposed[3] = values[1];    transposed[4] = values[4];    transposed[5] = values[7];
    transposed[6] = values[2];    transposed[7] = values[5];    transposed[8] = values[8];
    
    return transposed;
}

Vector3 Matrix3x3::GetScale() const
{
    return Vector3(values[0], values[4], values[8]);
}

Vector3 Matrix3x3::GetRotation() const
{
    float yaw, pitch, roll;
    
    if (values[1] > 0.998f) // Singularity at north pole
    {
        yaw = atan2(values[6], values[8]);
        pitch = PI/2;
        roll = 0;
    }
    else if (values[1] < -0.998f) // Singularity at south pole
    {
        yaw = atan2(-values[6], values[0]);
        pitch = -PI/2;
        roll = 0;
    }
    else
    {
        yaw = atan2(-values[2], values[0]);
        pitch = asin(values[1]);
        roll = atan2(-values[7], values[4]);
    }
    
    yaw = RadToDeg(yaw);
    pitch = RadToDeg(pitch);
    roll = RadToDeg(roll);
    
    return Vector3(yaw, pitch, roll);
}

float& Matrix3x3::operator[] (int i)
{
    if (i >= 0 && i < 9)
        return values[i];
    else
    {
        ERROR("Attempting to access invalid matrix index");
        return values[0];
    }
}

const float& Matrix3x3::operator[] (int i) const
{
    if (i >= 0 && i < 9)
        return values[i];
    else
    {
        ERROR("Attempting to access invalid matrix index");
        return values[0];
    }
}

const float* Matrix3x3::ToArray() const
{
    return values;
}

std::string Matrix3x3::ToString() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

Matrix3x3 Matrix3x3::Parse(const std::string &str)
{
    Matrix3x3 result;
    std::stringstream(str) >> result;
    return result;
}

std::istream& DrageEngine::operator>> (std::istream &stream, Matrix3x3 &matrix)
{
    char comma, pipe;
    stream >> matrix[0] >> comma >> matrix[3] >> comma >> matrix[6] >> pipe;
    stream >> matrix[1] >> comma >> matrix[4] >> comma >> matrix[7] >> pipe;
    stream >> matrix[2] >> comma >> matrix[5] >> comma >> matrix[8];
    return stream;
}

std::ostream& DrageEngine::operator<< (std::ostream &stream, const Matrix3x3 &matrix)
{
    stream << matrix[0] << ", " << matrix[3] << ", " << matrix[6] << " | ";
    stream << matrix[1] << ", " << matrix[4] << ", " << matrix[7] << " | ";
    stream << matrix[2] << ", " << matrix[5] << ", " << matrix[8];
    return stream;
}
