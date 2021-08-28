
#ifndef BILLBOARDCOMPONENT_H
#define BILLBOARDCOMPONENT_H

#include "Component.h"
#include "Material.h"
#include "Primitives.h"

namespace DrageEngine
{
    class BillboardComponent : public Component
    {
        public:
            void Init(const ParamList &params);
            void Render(Renderer *renderer, const Transform *transform = NULL) const;
        
        private:
            Material *m_material;
            Billboard m_mesh;
    };
}

#endif
