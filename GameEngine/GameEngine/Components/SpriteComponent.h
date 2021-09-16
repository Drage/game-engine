
#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "Component.h"
#include "Material.h"
#include "Renderable.h"

namespace DrageEngine
{
    class SpriteComponent : public Component
    {
        public:
            void Init(const ParamList &params);
            ~SpriteComponent();
        
        private:
            Material *material;
            Renderable *billboard;
    };
}

#endif
