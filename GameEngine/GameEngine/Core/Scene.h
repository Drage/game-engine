
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Entity.h"
#include "XMLDocument.h"

namespace DrageEngine
{
    class Scene
    {
        public:
            typedef std::vector<Entity*> EntityList;
        
            Scene();
            Scene(const std::string filename);
            ~Scene();
        
            void Clear();
        
            void Start();
            void Update();
        
            void Add(Entity *entity);
            bool Remove(Entity *entity);
        
            bool Load(const std::string &filename);
            Entity* LoadPrefab(const std::string &filename, ParamList *params = NULL);
        
            Entity* Find(const std::string &path, Entity *parent = NULL) const;
            const EntityList& GetEntities() const;
        
        private:
            void LoadEntities(const XMLDocument::Element *xml, Entity *parent = NULL, ParamList *params = NULL);
            EntityList entities;
    };
}

#endif
