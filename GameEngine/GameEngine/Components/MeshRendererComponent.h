
#ifndef MESHRENDERERCOMPONENT_H
#define MESHRENDERERCOMPONENT_H

#include <vector>
#include "Component.h"
#include "Model.h"
#include "Renderable.h"

namespace DrageEngine
{
    class MeshRendererComponent : public Component
    {
        public:
            void Init(const ParamList &params);
            ~MeshRendererComponent();
        
        private:
            Model *model;
            Material *material;
            std::vector<Renderable*> renderables;
    };
}

#endif
