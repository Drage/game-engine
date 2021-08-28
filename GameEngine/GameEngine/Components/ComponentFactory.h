
#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include <string>
#include "Factory.h"
#include "Component.h"

namespace DrageEngine
{
    class ComponentFactory
    {
        public:
            ComponentFactory();
            Component* Create(const std::string &type);
            bool IsSupported(const std::string &type) const;
        
            template<typename T>
            void Register(const std::string &type)
            {
                m_factory.Register<T>(type);
            }
        
        private:
            Factory<Component, std::string> m_factory;
    };
}

#endif
