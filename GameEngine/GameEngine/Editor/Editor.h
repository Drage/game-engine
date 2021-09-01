
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
        
            const std::vector<Entity*>& GetSelection() const;
            const std::vector<int> GetSelectionSceneIndexes() const;
        
            EditorCamera *camera;
        
        private:
            std::vector<Entity*> selection;
    };
}

#endif
