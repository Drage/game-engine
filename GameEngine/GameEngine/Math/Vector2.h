
#ifndef VECTOR2_H
#define VECTOR2_H

#include <string>

namespace DrageEngine
{
    class Vector2
    {
        friend std::istream& operator>>(std::istream &stream, Vector2 &vector);
        friend std::ostream& operator<<(std::ostream &stream, const Vector2 &vector);
        
        public:
            float x;
            float y;
        
            static const Vector2 RIGHT;
            static const Vector2 LEFT;
            static const Vector2 UP;
            static const Vector2 DOWN;
            static const Vector2 ZERO;
            static const Vector2 ONE;
        
            Vector2();
            Vector2(float x, float y);
            Vector2(float xy);
            Vector2(const Vector2 &other);
        
            Vector2 operator+ (const Vector2 &other) const;
            Vector2 operator- (const Vector2 &other) const;
            Vector2 operator- () const;
            Vector2 operator* (float operand) const;
            Vector2 operator/ (float operand) const;
            void operator+= (const Vector2 &other);
            void operator-= (const Vector2 &other);
            void operator*= (float operand);
            void operator/= (float operand);
            void operator= (const Vector2 &other);
            bool operator== (const Vector2 &other);
            bool operator!= (const Vector2 &other);

            float Magnitude() const;
            float MagnitudeSqr() const;
        
            Vector2 Normalize();
            Vector2 Normalized() const;
        
            static Vector2 Scale(const Vector2 &a, const Vector2 &b);
            static float Distance(const Vector2 &a, const Vector2 &b);
        
            static float Dot(const Vector2 &lhs, const Vector2 &rhs);
        
            Vector2 Rotate(float angle);
            static float Angle(const Vector2 &a, const Vector2 &b);
            static float SignedAngle(const Vector2 &a, const Vector2 &b);
        
            static Vector2 Reflect(const Vector2 &direction, const Vector2 &normal);
            static Vector2 Perpendicular(const Vector2 &direction);
        
            static Vector2 Lerp(const Vector2 &from, const Vector2 &to, float percent);
            static Vector2 MoveTowards(const Vector2 &current, const Vector2 &target, float maxDistanceDelta);
        
            static Vector2 Min(const Vector2 &a, const Vector2 &b);
            static Vector2 Max(const Vector2 &a, const Vector2 &b);
        
            float& operator[] (int i);
            const float& operator[] (int i) const;
            const float* ToArray() const;
            static Vector2 FromArray(float *values);
        
            std::string ToString() const;
            static Vector2 Parse(const std::string &str);
    };
}

#endif
