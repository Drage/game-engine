
#ifndef COMPONENT_H
#define COMPONENT_H

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
