
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector3.h"
#include "Matrix4x4.h"
#include "ParamList.h"

namespace DrageEngine
{
    class Transform
    {
        public:
            Vector3 position;
            Quaternion rotation;
            Vector3 scale;
        
            Transform();
            Transform(const Vector3 &position, const Quaternion &rotation, const Vector3 &scale);
            Transform(const Matrix4x4 &matrix);
            Transform(ParamList &params);
        
            void operator= (const Transform &other);
        
            Transform operator* (const Transform &other) const;
            void operator*= (const Transform &other);
        
            Transform operator+ (const Vector3 &translation) const;
            void operator+= (const Vector3 &translation);
        
            Transform operator* (const Quaternion &rotation) const;
            void operator*= (const Quaternion &rotation);
        
            Transform operator* (const Vector3 &scale) const;
            void operator*= (const Vector3 &scale);
        
            void Translate(const Vector3 &translation);
            void Rotate(const Quaternion &rotation);
            void Scale(const Vector3 &scale);
        
            void FromMatrix(const Matrix4x4 &matrix);
            Matrix4x4 ToMatrix() const;
            ParamList ToParamList() const;
        
            Vector3 Forward() const;
            Vector3 Back() const;
            Vector3 Up() const;
            Vector3 Down() const;
            Vector3 Left() const;
            Vector3 Right() const;
        
            Vector3 TransformDirection(const Vector3 &direction) const;
            Vector3 TransformPoint(const Vector3 &point) const;
            Vector3 InverseTransformDirection(const Vector3 &direction) const;
            Vector3 InverseTransformPoint(const Vector3 &point) const;
    };
}

#endif
