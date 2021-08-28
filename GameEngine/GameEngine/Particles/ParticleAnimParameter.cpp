
#include "ParticleAnimParameter.h"
#include "Random.h"

using namespace DrageEngine;

ParticleAnimParameter::ParticleAnimParameter()
{
    m_value = 0;
    m_variance = 0;
}

ParticleAnimParameter::ParticleAnimParameter(float value)
{
    m_value = value;
    m_variance = 0;
}

ParticleAnimParameter::ParticleAnimParameter(float value, float variance)
{
    m_value = value;
    m_variance = variance;
}

ParticleAnimParameter::ParticleAnimParameter(float value, float variance, const std::vector<Vector2> &curve)
{
    m_value = value;
    m_variance = variance;
    m_curve = curve;
}

float ParticleAnimParameter::GetBaseValue() const
{
    return m_value + Random::Float(-m_variance, m_variance);
}

float ParticleAnimParameter::GetAnimationValue(float t) const
{
    if (m_curve.size() == 0)
        return 1;
        
    std::vector<Vector2> tmp = m_curve;
    int i = (int)m_curve.size() - 1;
    while (i > 0)
    {
        for (int k = 0; k < i; k++)
            tmp[k] = tmp[k] + (tmp[k+1] - tmp[k]) * t;
        i--;
    }
    return tmp[0].y;
}
