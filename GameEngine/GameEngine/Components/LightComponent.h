
#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "Component.h"
#include "Light.h"

namespace DrageEngine
{
    class LightComponent : public Component, public Light
    {
        public:
            ~LightComponent();
            void Init(const ParamList &params);
            void Update();
    };
}

#endif
