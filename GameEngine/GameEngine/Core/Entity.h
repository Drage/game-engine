
#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include "Component.h"
#include "Transform.h"

namespace DrageEngine
{
    class Entity
    {
        public:
            Entity();
            ~Entity();
        
            void Init(ParamList &params);
            void Start();
            void Update();
        
            inline void Destroy() { destroyed = true; }
            inline bool IsDestroyed() const { return destroyed; }
        
            inline void SetActive(bool active) { this->active = active; }
            inline bool IsActive() { return active; }
        
            inline void SetName(const std::string &name) { this->name = name; }
            inline const std::string& GetName() { return name; };
        
            inline Entity* GetParent() const { return parent; }
            void SetParent(Entity *entity);
            void AddChild(Entity *entity);
            bool RemoveChild(Entity *entity);
            Entity* GetChild(const std::string &name);
            void GetAllChildrenInHierarchy(std::vector<Entity*> &children);
            const std::vector<Entity*>& GetChildren() const;
        
            void AddComponent(Component *component);
            bool RemoveComponent(Component *component);

            void SetPrefabRoot(Entity *entity);
            inline Entity* GetPrefabRoot() const { return prefabRoot; }
        
            template<typename T>
            T* GetComponent() const
            {
                for (int i = 0; i < components.size(); i++)
                {
                    T* cast = dynamic_cast<T*>(components[i]);
                    if (cast != nullptr)
                        return cast;
                }
                return NULL;
            }
        
            Transform GetGlobalTransform() const;
            Transform transform;
        
        private:        
            typedef std::vector<Entity*> EntityList;
            typedef std::vector<Component*> ComponentList;
            
            Entity *parent;
            EntityList children;
        
            ComponentList components;
        
            std::string id;
            std::string name;
            bool started;
            bool destroyed;
            bool active;

            Entity *prefabRoot;
    };
}

#endif
