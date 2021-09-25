
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
            Component();
            virtual void Init(const ParamList &params) {}
            virtual void Start() {}
            virtual void Update() {}
        
            void SetEnabled(bool enabled);
            bool IsEnabled() const;
        
            bool ExecuteInEditMode() const;
        
        protected:
            Entity *entity;
            Transform *transform;
            bool enabled;
            bool executeInEditMode;
        
        private:
            friend class Entity;
    };
}

#endif
