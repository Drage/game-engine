
#ifndef HIERARCHYWINDOW_H
#define HIERARCHYWINDOW_H

#include "EditorWindow.h"

namespace DrageEngine
{
    class HierarchyWindow : public EditorWindow
    {
        public:
            HierarchyWindow();
        
        private:
            void RenderContent();
    };
}

#endif
