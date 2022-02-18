
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
            void SetFocus(const Vector3 &focus, float distance);
        
        private:
            void Rotate(const Vector2 &delta);
            void Orbit(const Vector2 &delta);
            void Pan(const Vector2 &delta);
            void Zoom(float delta);
        
            Vector3 focus;
            float rotateSensitivity;
            float orbitSensitivity;
            float panSensitivity;
            float zoomSensitivity;

            float arrowKeyMoveSpeedSlow;
            float arrowKeyMoveSpeedFast;

            bool moveToTarget;
            float targetTransitionSpeed;
            Vector3 targetPosition;
            Vector3 targetFocus;
    };
}

#endif
