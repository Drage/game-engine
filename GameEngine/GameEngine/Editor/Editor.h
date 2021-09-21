
#ifndef EDITOR_H
#define EDITOR_H

#include <vector>
#include "Entity.h"
#include "EditorCamera.h"

namespace DrageEngine
{
    class Editor
    {
        public:
            Editor();
            ~Editor();
        
            void Update();
            void Render();
        
            bool IsSelected(Entity* entity) const;
            bool HasSelection() const;
        
            EditorCamera *camera;
        
        private:
            std::vector<Entity*> selection;
    };
}

#endif
