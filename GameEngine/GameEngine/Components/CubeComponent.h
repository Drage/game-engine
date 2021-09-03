
#ifndef CUBECOMPONENT_H
#define CUBECOMPONENT_H

#include "Component.h"
#include "Renderable.h"

namespace DrageEngine
{
    class CubeComponent : public Component
    {
        public:
            void Init(const ParamList &params);
            ~CubeComponent();
        
        private:
            Renderable *cube;
    };
}

#endif
