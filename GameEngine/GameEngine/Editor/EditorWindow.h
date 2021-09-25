
#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <string>

namespace DrageEngine
{
    class EditorWindow
    {
        public:
            EditorWindow(const std::string &name, const char *icon);
            void Render();
        
        protected:
            virtual void RenderContent();
            std::string windowName;
            bool windowOpen;
            bool hasMenuBar;
    };
}

#endif
