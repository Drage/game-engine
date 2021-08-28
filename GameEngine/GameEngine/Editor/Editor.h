
#ifndef EDITOR_H
#define EDITOR_H

#include "EditorCamera.h"

namespace DrageEngine
{
    class Editor
    {
        public:
            Editor();
            ~Editor();
        
            void Update();
        
            EditorCamera *camera;
        
        private:
        
    };
}

#endif
