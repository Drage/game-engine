
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "GameObject.h"
#include "Renderer.h"
#include "XMLDocument.h"

namespace DrageEngine
{
    class Scene
    {
        public:
            Scene();
            Scene(const std::string filename);
            ~Scene();
        
            void Clear();
        
            void Start();
            void Update();
            void Render(Renderer *renderer);
        
            void Add(GameObject *object);
            bool Remove(GameObject *object);
        
            bool Load(const std::string &filename);
            GameObject* LoadPrefab(const std::string &filename, ParamList *params = NULL);
        
            GameObject* Find(const std::string &path, GameObject *parent = NULL) const;
        
        private:
            void LoadObjects(const XMLDocument::Element *xml, GameObject *parent = NULL, ParamList *params = NULL);
        
            typedef std::vector<GameObject*> GameObjectList;
        
            GameObjectList objects;
    };
}

#endif
