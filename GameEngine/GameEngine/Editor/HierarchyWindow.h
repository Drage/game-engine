
#ifndef HIERARCHYWINDOW_H
#define HIERARCHYWINDOW_H

#include <vector>
#include "EditorWindow.h"
#include "Entity.h"

namespace DrageEngine
{
    class HierarchyWindow : public EditorWindow
    {
        public:
            HierarchyWindow();
        
        private:
            void RenderContent();
            void RenderEntities(const std::vector<Entity*> entities);
    };
}

#endif
