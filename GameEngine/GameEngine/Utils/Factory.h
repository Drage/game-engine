
#ifndef FACTORY_H
#define FACTORY_H

#include <map>
#include "Debug.h"

namespace DrageEngine
{
    template <class BaseType, class Key>
    class Factory
    {
        class Creator
        {
            public:
                virtual ~Creator(){}
                virtual BaseType* Create() = 0;
        };
        
        template <class DerivedType>
        class DerivedCreator : public Creator
        {
            public:
                BaseType* Create()
                {
                    return new DerivedType;
                }
        };
        
        public:
            template <class DerivedType>
            void Register(Key id)
            {
                Register(id, new DerivedCreator<DerivedType>());
            }
        
            bool IsSupported(Key id) const
            {
                return m_constructorMap.find(id) != m_constructorMap.end();
            }
        
            BaseType* Create(Key id)
            {
                Creator* creator = m_constructorMap[id];
                if (creator)
                    return creator->Create();
                else
                {
                    ERROR("Constructor for object type '" + id + "' does not exist in factory");
                    return NULL;
                }
            }
        
            virtual ~Factory()
            {
                typename std::map<Key, Creator*>::iterator i = m_constructorMap.begin();
                while (i != m_constructorMap.end())
                {
                    delete (*i).second;
                    i++;
                }
            }
        
        private:
            void Register(Key id, Creator* constructor)
            {
                m_constructorMap[id] = constructor;
            }
        
            std::map<Key, Creator*> m_constructorMap;
    };
}

#endif
