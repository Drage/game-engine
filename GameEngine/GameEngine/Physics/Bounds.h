
#ifndef BOUNDS_H
#define BOUNDS_H

#include "Vector3.h"

namespace DrageEngine
{
    struct Bounds
    {
        Vector3 min;
        Vector3 max;
        
        Bounds();
        Bounds(const Vector3 &min, const Vector3 &max);
        
        Vector3 GetDimensions();
    };
}

#endif
