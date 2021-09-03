
#ifndef PLANECOMPONENT_H
#define PLANECOMPONENT_H

#include "Component.h"
#include "Renderable.h"

namespace DrageEngine
{
    class PlaneComponent : public Component
    {
        public:
            void Init(const ParamList &params);
            ~PlaneComponent();
        
        private:
            Renderable *plane;
    };
}

#endif
