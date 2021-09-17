
#ifndef SCREENSPACEANCHORCOMPONENT_h
#define SCREENSPACEANCHORCOMPONENT_h

#include "Component.h"
#include "Vector2.h"

namespace DrageEngine
{
    class ScreenSpaceAnchorComponent : public Component
    {
        public:
            void Init(const ParamList &params);
            void Update();
        
        private:
            Vector2 anchor;
    };
}

#endif
