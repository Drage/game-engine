
#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Vector4.h"
#include "Bounds.h"
#include "Matrix4x4.h"

namespace DrageEngine
{
    class Frustum
    {
        public:
            void Update(const Vector3 &position, const Vector3 &direction, float near, float far, float fov);
            bool CheckSphere(const Vector3 &position, float radius) const;
            bool CheckBounds(const Bounds &bounds, const Matrix4x4 &modelMatrix) const;
        
        private:
            Vector4 ComputePlane(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3) const;
            Vector4 planes[6];
    };
}

#endif
