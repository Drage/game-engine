
#ifndef COMPONENT_H
#define COMPONENT_H

#include "Renderer.h"
#include "Transform.h"
#include "ParamList.h"

namespace DrageEngine
{
    class Entity;

    class Component
    {
        public:
            virtual void Init(const ParamList &params) {}
            virtual void Start() {}
            virtual void Update() {}
            virtual void Render(Renderer *renderer, const Transform *transform = NULL) const {}
        
            void SetEnabled(bool enabled);
            bool IsEnabled() const;
        
        protected:
            Entity *entity;
            Transform *transform;
            
        
        private:
            friend class Entity;
        
            bool enabled;
    };
}

#endif
