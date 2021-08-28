
#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <GameEngine.h>

using namespace DrageEngine;

class CameraController : public Component
{
    public:
        void Init(const ParamList &params);
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

#endif
