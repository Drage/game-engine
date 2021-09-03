
#ifndef BILLBOARDCOMPONENT_H
#define BILLBOARDCOMPONENT_H

#include "Component.h"
#include "Renderable.h"

namespace DrageEngine
{
    class BillboardComponent : public Component
    {
        public:
            void Init(const ParamList &params);
            ~BillboardComponent();
        
        private:
            Renderable *billboard;
    };
}

#endif
