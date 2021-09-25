
#ifndef INSPECTORWINDOW_H
#define INSPECTORWINDOW_H

#include "EditorWindow.h"

namespace DrageEngine
{
    class InspectorWindow : public EditorWindow
    {
        public:
            InspectorWindow();
        
        private:
            void RenderContent();
    };
}

#endif
