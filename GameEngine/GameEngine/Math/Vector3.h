
#ifndef VECTOR3_H
#define VECTOR3_H

#include <string>
#include "Vector2.h"

namespace DrageEngine
{
    class Vector3
    {
        friend std::istream& operator>>(std::istream &stream, Vector3 &vector);
        friend std::ostream& operator<<(std::ostream &stream, const Vector3 &vector);
        
        public:
            float x;
            float y;
            float z;
        
            static const Vector3 RIGHT;
            static const Vector3 LEFT;
            static const Vector3 UP;
            static const Vector3 DOWN;
            static const Vector3 FORWARD;
            static const Vector3 BACK;
            static const Vector3 ZERO;
            static const Vector3 ONE;
        
            Vector3();
            Vector3(float x, float y, float z);
            Vector3(float xyz);
            Vector3(const Vector3 &other);
            Vector3(const Vector2 &vec2, float z = 0);

            Vector3 operator+ (const Vector3 &other) const;
            Vector3 operator- (const Vector3 &other) const;
            Vector3 operator- () const;
            Vector3 operator* (float operand) const;
            Vector3 operator/ (float operand) const;
            void operator+= (const Vector3 &other);
            void operator-= (const Vector3 &other);
            void operator*= (float operand);
            void operator/= (float operand);
            void operator= (const Vector3 &other);
            bool operator== (const Vector3 &other);
            bool operator!= (const Vector3 &other);
            bool operator< (const Vector3 &other) const;
            bool operator> (const Vector3 &other) const;
        
            float Magnitude() const;
            float MagnitudeSqr() const;
        
            Vector3 Normalize();
            Vector3 Normalized() const;
        
            static Vector3 Scale(const Vector3 &a, const Vector3 &b);
            static float Distance(const Vector3 &a, const Vector3 &b);
        
            static float Dot(const Vector3 &lhs, const Vector3 &rhs);
            static Vector3 Cross(const Vector3 &lhs, const Vector3 &rhs);
        
            static float Angle(const Vector3 &a, const Vector3 &b);
            static float SignedAngle(const Vector3 &a, const Vector3 &b, const Vector3 &axis);
        
            static Vector3 Reflect(const Vector3 &direction, const Vector3 &normal);
            static Vector3 Project(const Vector3 &vector, const Vector3 &normal);
        
            static Vector3 Lerp(const Vector3 &from, const Vector3 &to, float percent);
            static Vector3 MoveTowards(const Vector3 &current, const Vector3 &target, float maxDistanceDelta);
        
            static Vector3 Min(const Vector3 &a, const Vector3 &b);
            static Vector3 Max(const Vector3 &a, const Vector3 &b);
        
            float& operator[] (int i);
            const float& operator[] (int i) const;
            const float* ToArray() const;
            static Vector3 FromArray(float *values);
        
            std::string ToString() const;
            static Vector3 Parse(const std::string &str);
    };
}

#endif
