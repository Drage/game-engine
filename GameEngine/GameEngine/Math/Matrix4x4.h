
#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "Vector3.h"
#include "Quaternion.h"

namespace DrageEngine
{
    class Matrix3x3;

    class Matrix4x4
    {
        friend std::istream& operator>>(std::istream &stream, Matrix4x4 &matrix);
        friend std::ostream& operator<<(std::ostream &stream, const Matrix4x4 &matrix);
        
        public:
            Matrix4x4();
            Matrix4x4(const Matrix3x3 &other);
        
            void SetIdentity();
            static const Matrix4x4 IDENTITY;
        
            Matrix4x4 operator* (const Matrix4x4 &other) const;
            Matrix4x4 operator+ (const Matrix4x4 &other) const;
            Matrix4x4 operator- (const Matrix4x4 &other) const;
            Vector3 operator* (const Vector3 &point) const;
            void operator*= (const Matrix4x4 &other);
            void operator+= (const Matrix4x4 &other);
            void operator-= (const Matrix4x4 &other);
        
            void SetTranslation(const Vector3 &translation);
            void SetScale(const Vector3 &scale);
            void SetRotation(const Vector3 &axis, float angle);
            void SetRotation(const Vector3 &rotation);
            void SetRotation(const Vector3 &right, const Vector3 &up, const Vector3 &forward);
            void SetRotation(const Quaternion &quaternion);
    
            void Invert();
            Matrix4x4 Inverse() const;
        
            void Transpose();
            Matrix4x4 Transposed() const;
        
            void SetProjectionPerspective(float fov, float apectRatio, float nearClip, float farClip);
            void SetProjectionOrthographic(int width, int height);
            void SetView(const Vector3 &position, const Vector3 &focus, const Vector3 &up = Vector3(0, 1, 0));
        
            Vector3 GetTranslation() const;
            Vector3 GetScale() const;
            Quaternion GetRotation() const;
        
            float& operator[] (int i);
            const float& operator[] (int i) const;
            const float* ToArray() const;
        
            std::string ToString() const;
            static Matrix4x4 Parse(const std::string &str);
        
        private:
            float values[16];
    };
}

#endif
