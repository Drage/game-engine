
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
        
        private:
            void Rotate(const Vector2 &delta);
            void Pan(const Vector2 &delta);
            void Zoom(float delta);
        
            Vector3 m_focus;
            float m_rotateSensitivity;
            float m_panSensitivity;
            float m_zoomSensitivity;
    };
}

#endif
