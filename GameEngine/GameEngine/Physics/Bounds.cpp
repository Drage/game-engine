
#include "Bounds.h"

using namespace DrageEngine;

Bounds::Bounds()
{
}

Bounds::Bounds(const Vector3 &min, const Vector3 &max)
{
    this->min = min;
    this->max = max;
}

Vector3 Bounds::GetDimensions()
{
    return max - min;
}
