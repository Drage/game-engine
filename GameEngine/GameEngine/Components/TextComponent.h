
#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include "Component.h"
#include "Material.h"
#include "Renderable.h"

namespace DrageEngine
{
    class TextComponent : public Component
    {
        public:
            void Init(const ParamList &params);
            ~TextComponent();
            Renderable *billboard;
        
        private:
            Material *material;
            
    };
}

#endif
