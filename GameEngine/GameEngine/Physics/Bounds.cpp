
#include "Bounds.h"
#include "MathUtils.h"

using namespace DrageEngine;

Bounds::Bounds()
{
    min = Vector3(0);
    max = Vector3(0);
    radius = 0;
}

Bounds::Bounds(const Vector3 &min, const Vector3 &max, float radius)
{
    this->min = min;
    this->max = max;
    this->radius = radius;
}

Vector3 Bounds::GetDimensions()
{
    return max - min;
}

Bounds Bounds::Combine(const Bounds &a, const Bounds &b)
{
    Vector3 min = Vector3(Min(a.min.x, b.min.x), Min(a.min.y, b.min.y), Min(a.min.z, b.min.z));
    Vector3 max = Vector3(Max(a.max.x, b.max.x), Max(a.max.y, b.max.y), Max(a.max.z, b.max.z));
    float radius = Max(a.radius, b.radius);
    return Bounds(min, max, radius);
}
