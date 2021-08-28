
#ifndef PARTICLEANIMPARAMETER_H
#define PARTICLEANIMPARAMETER_H

#include <vector>
#include "Vector2.h"

namespace DrageEngine
{
    class ParticleAnimParameter
    {
        public:
            ParticleAnimParameter();
            ParticleAnimParameter(float value);
            ParticleAnimParameter(float value, float variance);
            ParticleAnimParameter(float value, float variance, const std::vector<Vector2> &curve);
        
            float GetBaseValue() const;
            float GetAnimationValue(float t) const;
        
        private:
            float m_value;
            float m_variance;
            std::vector<Vector2> m_curve;
    };
}

#endif
