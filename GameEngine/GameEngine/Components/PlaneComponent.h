
#ifndef PLANECOMPONENT_H
#define PLANECOMPONENT_H

#include "Component.h"
#include "Material.h"
#include "Primitives.h"

namespace DrageEngine
{
    class PlaneComponent : public Component
    {
        public:
            void Init(const ParamList &params);
            void Render(Renderer *renderer, const Transform *transform = NULL) const;
        
        private:
            Material *m_material;
            Plane m_mesh;
    };
}

#endif
