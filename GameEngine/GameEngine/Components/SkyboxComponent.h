
#ifndef SKYBOXCOMPONENT_H
#define SKYBOXCOMPONENT_H

#include "Component.h"
#include "Material.h"
#include "Primitives.h"

namespace DrageEngine
{
    class SkyboxComponent : public Component
    {
        public:
            void Init(const ParamList &params);
            void Render(Renderer *renderer, const Transform *transform = NULL) const;
        
        private:
            Material *material;
            Cube mesh;
    };
}

#endif
