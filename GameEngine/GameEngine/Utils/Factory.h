
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
                return constructorMap.find(id) != constructorMap.end();
            }
        
            BaseType* Create(Key id)
            {
                Creator* creator = constructorMap[id];
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
                typename std::map<Key, Creator*>::iterator i = constructorMap.begin();
                while (i != constructorMap.end())
                {
                    delete (*i).second;
                    i++;
                }
            }
        
        private:
            void Register(Key id, Creator* constructor)
            {
                constructorMap[id] = constructor;
            }
        
            std::map<Key, Creator*> constructorMap;
    };
}

#endif
