
#ifndef SPHERECOMPONENT_H
#define SPHERECOMPONENT_H

#include "Component.h"
#include "Renderable.h"

namespace DrageEngine
{
    class SphereComponent : public Component
    {
        public:
            void Init(const ParamList &params);
            ~SphereComponent();
        
        private:
            Renderable *sphere;
    };
}

#endif
