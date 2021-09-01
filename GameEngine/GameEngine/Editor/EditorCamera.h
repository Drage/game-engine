
#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H

#include "Camera.h"
#include "Vector2.h"

namespace DrageEngine
{
    class EditorCamera : public Camera
    {
        public:
            EditorCamera();
            void Update();
            void SetFocus(const Vector3 &focus);
        
        private:
            void Rotate(const Vector2 &delta);
            void Pan(const Vector2 &delta);
            void Zoom(float delta);
        
            Vector3 focus;
            float rotateSensitivity;
            float panSensitivity;
            float zoomSensitivity;
    };
}

#endif
