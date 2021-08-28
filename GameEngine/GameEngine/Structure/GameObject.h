
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <string>
#include "Component.h"
#include "Transform.h"
#include "Renderer.h"
#include "Debug.h"

namespace DrageEngine
{
    class GameObject
    {
        public:
            GameObject();
            ~GameObject();
        
            void Init(ParamList &params);
            void Start();
            void Update();
            void Render(Renderer *renderer, const Transform *transform = NULL) const;
        
            inline void Destroy() { destroyed = true; }
            inline bool IsDestroyed() const { return destroyed; }
        
            inline void SetActive(bool active) { active = active; }
            inline bool IsActive() { return active; }
        
            inline const std::string& GetName() { return name; };
        
            inline const GameObject* GetParent() const { return parent; }
            void SetParent(GameObject *obj);
            void AddChild(GameObject *obj);
            bool RemoveChild(GameObject *obj);
            GameObject* GetChild(const std::string &name);
        
            void AddComponent(Component *component);
            bool RemoveComponent(Component *component);
        
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
        
            Transform transform;
        
        private:        
            typedef std::vector<GameObject*> GameObjectList;
            typedef std::vector<Component*> ComponentList;
        
            GameObject *parent;
            GameObjectList children;
        
            ComponentList components;
        
            bool started;
            bool destroyed;
            bool active;
            std::string name;
    };
}

#endif
