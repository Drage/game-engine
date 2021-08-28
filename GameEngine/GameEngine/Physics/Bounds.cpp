
#include "Bounds.h"

using namespace DrageEngine;

Bounds::Bounds()
{
    min = Vector3(std::numeric_limits<float>::max());
    max = Vector3(-std::numeric_limits<float>::max());
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
