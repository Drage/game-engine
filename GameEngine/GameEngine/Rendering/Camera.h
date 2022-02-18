
#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "Matrix4x4.h"
#include "Frustum.h"

namespace DrageEngine
{
    class Camera
    {
        public:
            Camera();
            Camera(const Vector3 &pos, const Vector3 &dir, float fov = 60, float nearClip = 0.1f, float farClip = 1000);
            virtual ~Camera() {}
        
            void SetPosition(const Vector3 &position);
            void SetDirection(const Vector3 &direction);
            void SetFOV(float fov);
            void SetNearClip(float nearClip);
            void SetFarClip(float farClip);
        
            void SetProjectionPerspective(float fov, float nearClip, float farClip);
        
            Vector3 GetPosition() const;
            Vector3 GetDirection() const;
            float GetFOV() const;
            float GetNearClip() const;
            float GetFarClip() const;

            Matrix4x4 GetViewMatrix() const;
            Matrix4x4 GetProjectionMatrix() const;

            Vector3 WorldToScreenPoint(const Vector3 &point);
        
            const Frustum& GetFrustum();
        
        protected:
            Vector3 position;
            Vector3 direction;
            float fov;
            float nearClip;
            float farClip;
            Frustum frustum;
        };
}

#endif
