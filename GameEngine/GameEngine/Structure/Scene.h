
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Entity.h"
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
            void Render(Renderer *renderer) const;
        
            void Add(Entity *entity);
            bool Remove(Entity *entity);
        
            bool Load(const std::string &filename);
            Entity* LoadPrefab(const std::string &filename, ParamList *params = NULL);
        
            Entity* Find(const std::string &path, Entity *parent = NULL) const;
            Entity* GetEntityByIndex(int index) const;
            int GetIndexOfEntity(const Entity* entity) const;
        
        private:
            typedef std::vector<Entity*> EntityList;
            void LoadEntities(const XMLDocument::Element *xml, Entity *parent = NULL, ParamList *params = NULL);
            EntityList entities;
    };
}

#endif
