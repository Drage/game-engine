
#ifndef VECTOR4_H
#define VECTOR4_H

#include <string>
#include "Vector3.h"

namespace DrageEngine
{
    class Vector4
    {
        friend std::istream& operator>>(std::istream &stream, Vector4 &vector);
        friend std::ostream& operator<<(std::ostream &stream, const Vector4 &vector);
        
        public:
            float x;
            float y;
            float z;
            float w;
        
            static const Vector4 ZERO;
            static const Vector4 ONE;
        
            Vector4();
            Vector4(float x, float y, float z, float w);
            Vector4(float xyzw);
            Vector4(const Vector4 &other);
            Vector4(const Vector3 &vec3, float w = 0);
        
            Vector4 operator+ (const Vector4 &other) const;
            Vector4 operator- (const Vector4 &other) const;
            Vector4 operator- () const;
            Vector4 operator* (float operand) const;
            Vector4 operator/ (float operand) const;
            void operator+= (const Vector4 &other);
            void operator-= (const Vector4 &other);
            void operator*= (float operand);
            void operator/= (float operand);
            void operator= (const Vector4 &other);
            bool operator== (const Vector4 &other);
            bool operator!= (const Vector4 &other);

            float Magnitude() const;
            float MagnitudeSqr() const;
        
            Vector4 Normalize();
            Vector4 Normalized() const;
        
            static float Distance(const Vector4 &a, const Vector4 &b);
        
            static float Dot(const Vector4 &lhs, const Vector4 &rhs);
        
            static Vector4 Lerp(const Vector4 &from, const Vector4 &to, float percent);
            static Vector4 MoveTowards(const Vector4 &current, const Vector4 &target, float maxDistanceDelta);
        
            static Vector4 Min(const Vector4 &a, const Vector4 &b);
            static Vector4 Max(const Vector4 &a, const Vector4 &b);
        
            float& operator[] (int i);
            const float& operator[] (int i) const;
            const float* ToArray() const;
            static Vector4 FromArray(float *values);
        
            std::string ToString() const;
            static Vector4 Parse(const std::string &str);
    };
}

#endif
