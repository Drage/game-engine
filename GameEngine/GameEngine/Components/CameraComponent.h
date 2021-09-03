
#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "Component.h"
#include "Camera.h"

namespace DrageEngine
{
    class CameraComponent : public Component, public Camera
    {
        public:
            void Init(const ParamList &params);
            void Start();
            void Update();
    };
}

#endif
