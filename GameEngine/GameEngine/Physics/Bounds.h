
#ifndef BOUNDS_H
#define BOUNDS_H

#include "Vector3.h"

namespace DrageEngine
{
    struct Bounds
    {
        Vector3 min;
        Vector3 max;
        float radius;
        
        Bounds();
        Bounds(const Vector3 &min, const Vector3 &max, float radius);
        
        Vector3 GetDimensions();

        static Bounds Combine(const Bounds &a, const Bounds &b);
    };
}

#endif
