
#ifndef SPHERECOMPONENT_H
#define SPHERECOMPONENT_H

#include "Component.h"
#include "Material.h"
#include "Primitives.h"

namespace DrageEngine
{
    class SphereComponent : public Component
    {
        public:
            void Init(const ParamList &params);
            void Render(Renderer *renderer, const Transform *transform = NULL) const;
        
        private:
            Material *material;
            Sphere mesh;
    };
}

#endif
