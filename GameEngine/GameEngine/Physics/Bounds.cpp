
#include "Bounds.h"

using namespace DrageEngine;

Bounds::Bounds()
{
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
