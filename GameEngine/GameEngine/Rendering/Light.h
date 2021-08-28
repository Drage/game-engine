
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
        
            Type type;
            Vector4 position;
            Color color;
            float attenuation;
            float ambientCoefficient;
            Vector3 coneDirection;
            float coneAngle;
    };
}

#endif
