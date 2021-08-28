
#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "Shader.h"

namespace DrageEngine
{
    class Light
    {
        public:
            enum Type { DIRECTIONAL, POINT, SPOTLIGHT };
        
            void ApplyUniforms(const Shader *shader, int index) const;
        
        protected:
            Light();
        
            Type m_type;
            Vector4 m_position;
            Color m_color;
            float m_attenuation;
            float m_ambientCoefficient;
            Vector3 m_coneDirection;
            float m_coneAngle;
    };
}

#endif
