
#include "ParticleAnimParameter.h"
#include "Random.h"

using namespace DrageEngine;

ParticleAnimParameter::ParticleAnimParameter()
{
    value = 0;
    variance = 0;
}

ParticleAnimParameter::ParticleAnimParameter(float value)
{
    this->value = value;
    this->variance = 0;
}

ParticleAnimParameter::ParticleAnimParameter(float value, float variance)
{
    this->value = value;
    this->variance = variance;
}

ParticleAnimParameter::ParticleAnimParameter(float value, float variance, const std::vector<Vector2> &curve)
{
    this->value = value;
    this->variance = variance;
    this->curve = curve;
}

float ParticleAnimParameter::GetBaseValue() const
{
    return value + Random::Float(-variance, variance);
}

float ParticleAnimParameter::GetAnimationValue(float t) const
{
    if (curve.size() == 0)
        return 1;
        
    std::vector<Vector2> tmp = curve;
    int i = (int)curve.size() - 1;
    while (i > 0)
    {
        for (int k = 0; k < i; k++)
            tmp[k] = tmp[k] + (tmp[k+1] - tmp[k]) * t;
        i--;
    }
    return tmp[0].y;
}
