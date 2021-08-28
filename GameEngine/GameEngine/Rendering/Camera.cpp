
#include "Camera.h"
#include "Application.h"

using namespace DrageEngine;

Camera::Camera()
{
    m_position = Vector3(0, 0, 10);
    m_direction = Vector3(0, 0, -1);
    m_fov = 60;
    m_nearClip = 0.1f;
    m_farClip = 1000.0f;
}

Camera::Camera(const Vector3 &position, const Vector3 &direction, float fov, float nearClip, float farClip)
{
    m_position = position;
    m_direction = direction;
    m_fov = fov;
    m_nearClip = nearClip;
    m_farClip = farClip;
}

void Camera::SetPosition(const Vector3 &position)
{
    m_position = position;
}

void Camera::SetDirection(const Vector3 &direction)
{
    m_direction = direction;
}

void Camera::SetFOV(float fov)
{
    m_fov = fov;
}

void Camera::SetNearClip(float nearClip)
{
    m_nearClip = nearClip;
}

void Camera::SetFarClip(float farClip)
{
    m_farClip = farClip;
}

void Camera::SetProjectionPerspective(float fov, float nearClip, float farClip)
{
    m_fov = fov;
    m_nearClip = nearClip;
    m_farClip = farClip;
}

Vector3 Camera::GetPosition() const
{
    return m_position;
}

Vector3 Camera::GetDirection() const
{
    return m_direction;
}

float Camera::GetFOV() const
{
    return m_fov;
}

float Camera::GetNearClip() const
{
    return m_nearClip;
}

float Camera::GetFarClip() const
{
    return m_farClip;
}

Matrix4x4 Camera::GetViewMatrix() const
{
    Matrix4x4 view;
    view.SetView(m_position, m_direction, Vector3::UP);
    return view;
}

Matrix4x4 Camera::GetProjectionMatrix() const
{
    Matrix4x4 projection;
    projection.SetProjectionPerspective(m_fov, app->window->GetAspectRatio(), m_nearClip, m_farClip);
    return projection;
}
