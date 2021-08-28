
#ifndef MESHRENDERERCOMPONENT_H
#define MESHRENDERERCOMPONENT_H

#include "Component.h"
#include "Model.h"

namespace DrageEngine
{
    class MeshRendererComponent : public Component
    {
        public:
            void Init(const ParamList &params);
            void Render(Renderer *renderer, const Transform *transform = NULL) const;
        
        private:
            Model *model;
            Material *material;
    };
}

#endif
