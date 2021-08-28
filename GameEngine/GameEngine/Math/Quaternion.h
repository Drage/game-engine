
#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector3.h"

namespace DrageEngine
{
    class Quaternion
    {
        friend std::istream& operator>>(std::istream &stream, Quaternion &quaternion);
        friend std::ostream& operator<<(std::ostream &stream, const Quaternion &quaternion);
        
        public:
            float w, x, y, z;
        
            Quaternion();
            Quaternion(float x, float y, float z, float w);
            Quaternion(const Quaternion &other);
        
            static const Quaternion IDENTITY;
            void SetIdentity();
            
            Quaternion operator* (const Quaternion &other) const;
            void operator*= (const Quaternion &other);
            Vector3 operator* (const Vector3 &vector) const;
            Quaternion operator+ (const Quaternion &other) const;
            Quaternion operator+= (const Quaternion &other);
            Quaternion operator- (const Quaternion &other) const;
            Quaternion operator-= (const Quaternion &other);
            Quaternion operator* (float scale) const;
            Quaternion operator*= (float scale);
            Quaternion operator/ (float scale) const;
            Quaternion operator/= (float scale);
            Quaternion operator- () const;
            void operator= (const Quaternion &other);
        
            float Magnitude() const;
            float SqrMagnitude() const;
        
            void Normalize();
            Quaternion Normalized() const;
        
            Quaternion Conjugate() const;
            void Invert();
            Quaternion Inverse() const;
        
            static float Dot(const Quaternion &lhs, const Quaternion &rhs);
        
            static Quaternion FromEuler(float pitch, float yaw, float roll);
            static Quaternion FromEuler(const Vector3 &rotation);
            static Quaternion FromAxisAngle(const Vector3 &axis, float angle);
            static Quaternion FromLookDirection(const Vector3 &direction);
            static Quaternion FromToRotation(const Vector3 &fromDirection, const Vector3 &toDirection);
        
            Vector3 ToEuler() const;
            void ToAxisAngle(Vector3 *axis, float *angle) const;
        
            static Quaternion Slerp(const Quaternion &from, const Quaternion &to, float percent);
            static Quaternion Lerp(const Quaternion &from, const Quaternion &to, float percent);
        
            std::string ToString() const;
            static Quaternion Parse(const std::string &str);
    };
}

#endif
