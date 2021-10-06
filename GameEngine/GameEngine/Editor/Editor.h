
#ifndef EDITOR_H
#define EDITOR_H

#include <vector>
#include "Entity.h"
#include "EditorCamera.h"
#include "EditorWindow.h"

namespace DrageEngine
{
    class Editor
    {
        public:
            Editor();
            ~Editor();
        
            void HandleInput(const SDL_Event* event);
        
            void Update();
            void Render();
        
            bool IsSelected(Entity* entity) const;
            bool HasSelection() const;
            const std::vector<Entity*>& GetSelection() const;
            void SetSelection(Entity* entity);
        
            EditorCamera *camera;
        
        private:
            std::vector<Entity*> selection;
            std::vector<EditorWindow*> windows;
    };
}

#endif
