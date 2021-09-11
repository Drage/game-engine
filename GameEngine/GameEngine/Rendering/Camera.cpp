
#include "Camera.h"
#include "Application.h"

using namespace DrageEngine;

Camera::Camera()
{
    position = Vector3(0, 0, 10);
    direction = Vector3(0, 0, -1);
    fov = 60;
    nearClip = 0.1f;
    farClip = 10000.0f;
}

Camera::Camera(const Vector3 &position, const Vector3 &direction, float fov, float nearClip, float farClip)
{
    this->position = position;
    this->direction = direction;
    this->fov = fov;
    this->nearClip = nearClip;
    this->farClip = farClip;
}

void Camera::SetPosition(const Vector3 &position)
{
    this->position = position;
}

void Camera::SetDirection(const Vector3 &direction)
{
    this->direction = direction;
}

void Camera::SetFOV(float fov)
{
    this->fov = fov;
}

void Camera::SetNearClip(float nearClip)
{
    this->nearClip = nearClip;
}

void Camera::SetFarClip(float farClip)
{
    this->farClip = farClip;
}

void Camera::SetProjectionPerspective(float fov, float nearClip, float farClip)
{
    this->fov = fov;
    this->nearClip = nearClip;
    this->farClip = farClip;
}

Vector3 Camera::GetPosition() const
{
    return position;
}

Vector3 Camera::GetDirection() const
{
    return direction;
}

float Camera::GetFOV() const
{
    return fov;
}

float Camera::GetNearClip() const
{
    return nearClip;
}

float Camera::GetFarClip() const
{
    return farClip;
}

Matrix4x4 Camera::GetViewMatrix() const
{
    Matrix4x4 view;
    view.SetView(position, direction, Vector3::UP);
    return view;
}

Matrix4x4 Camera::GetProjectionMatrix() const
{
    Matrix4x4 projection;
    projection.SetProjectionPerspective(fov, app->window->GetAspectRatio(), nearClip, farClip);
    return projection;
}

const Frustum& Camera::GetFrustum()
{
    static Vector3 lastPosition;
    static Vector3 lastDirection;
    static float lastFov;
    
    if (position != lastPosition || direction != lastDirection || fov != lastFov)
    {
        lastPosition = position;
        lastDirection = direction;
        lastFov = fov;
        frustum.Update(position, direction, nearClip, farClip, fov);
    }
    
    return frustum;
}
