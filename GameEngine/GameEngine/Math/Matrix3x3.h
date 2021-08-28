
#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include "Vector3.h"
#include "Quaternion.h"

namespace DrageEngine
{
    class Matrix4x4;

    class Matrix3x3
    {
        friend std::istream& operator>>(std::istream &stream, Matrix3x3 &matrix);
        friend std::ostream& operator<<(std::ostream &stream, const Matrix3x3 &matrix);
        
        public:
            Matrix3x3();
            Matrix3x3(const Matrix4x4 &other);
        
            void SetIdentity();
            static const Matrix3x3 IDENTITY;
        
            Matrix3x3 operator* (const Matrix3x3 &other) const;
            Matrix3x3 operator+ (const Matrix3x3 &other) const;
            Matrix3x3 operator- (const Matrix3x3 &other) const;
            Vector3 operator* (const Vector3 &point) const;
            void operator*= (const Matrix3x3 &other);
            void operator+= (const Matrix3x3 &other);
            void operator-= (const Matrix3x3 &other);
        
            void SetScale(const Vector3 &scale);
            void SetRotation(const Vector3 &axis, float angle);
            void SetRotation(const Vector3 &rotation);
            void SetRotation(const Vector3 &right, const Vector3 &up, const Vector3 &forward);
            void SkewSymetric(const Vector3 &skew);
        
            void Invert();
            Matrix3x3 Inverse() const;
        
            void Orthonormalize();
            Matrix3x3 Orthonormalized() const;
        
            void Transpose();
            Matrix3x3 Transposed() const;
        
            Vector3 GetScale() const;
            Vector3 GetRotation() const;
        
            float& operator[] (int i);
            const float& operator[] (int i) const;
            const float* ToArray() const;
        
            std::string ToString() const;
            static Matrix3x3 Parse(const std::string &str);
        
        private:
            float values[9];
    };
}

#endif
