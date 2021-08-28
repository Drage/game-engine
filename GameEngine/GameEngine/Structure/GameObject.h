
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
        
            inline void Destroy() { m_destroyed = true; }
            inline bool IsDestroyed() const { return m_destroyed; }
        
            inline void SetActive(bool active) { m_active = active; }
            inline bool IsActive() { return m_active; }
        
            inline const std::string& GetName() { return m_name; };
        
            inline const GameObject* GetParent() const { return m_parent; }
            void SetParent(GameObject *obj);
            void AddChild(GameObject *obj);
            bool RemoveChild(GameObject *obj);
            GameObject* GetChild(const std::string &name);
        
            void AddComponent(Component *component);
            bool RemoveComponent(Component *component);
        
            template<typename T>
            T* GetComponent() const
            {
                for (int i = 0; i < m_components.size(); i++)
                {
                    T* cast = dynamic_cast<T*>(m_components[i]);
                    if (cast != nullptr)
                        return cast;
                }
                return NULL;
            }
        
            Transform transform;
        
        private:        
            typedef std::vector<GameObject*> GameObjectList;
            typedef std::vector<Component*> ComponentList;
        
            GameObject *m_parent;
            GameObjectList m_children;
        
            ComponentList m_components;
        
            bool m_started;
            bool m_destroyed;
            bool m_active;
            std::string m_name;
    };
}

#endif
