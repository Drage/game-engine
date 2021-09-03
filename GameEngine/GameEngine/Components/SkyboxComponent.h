
#ifndef SKYBOXCOMPONENT_H
#define SKYBOXCOMPONENT_H

#include "Component.h"
#include "Renderable.h"

namespace DrageEngine
{
    class SkyboxComponent : public Component
    {
        public:
            void Init(const ParamList &params);
            ~SkyboxComponent();
        
        private:
            Renderable *skybox;
    };
}

#endif
