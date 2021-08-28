
#include "EditorCamera.h"
#include "Input.h"
#include "Application.h"
#include "MathUtils.h"

using namespace DrageEngine;

EditorCamera::EditorCamera()
{
    m_rotateSensitivity = 0.8f;
    m_panSensitivity = 0.2f;
    m_zoomSensitivity = 0.5f;
    m_focus = Vector3(0, 0, 0);
}

void EditorCamera::Update()
{
    Input *input = app->input;
    Vector2 mouseDelta = input->GetMouseDelta();
    
    float distance = (m_position - m_focus).Magnitude();
    
    if (input->GetMouseButton(Mouse::LEFT))
    {
        if (input->GetKey(Key::LALT) || input->GetKey(Key::RALT))
            Rotate(mouseDelta * m_rotateSensitivity * Time::DeltaTime());
        else if (input->GetKey(Key::LCOMMAND) || input->GetKey(Key::RCOMMAND))
            Zoom(mouseDelta.y * m_zoomSensitivity * Time::DeltaTime() * distance);
        else
            Pan(mouseDelta * m_panSensitivity * Time::DeltaTime() * distance);
    }
    else if (input->GetMouseButton(Mouse::MIDDLE))
    {
        Zoom(mouseDelta.y * m_zoomSensitivity * Time::DeltaTime() * distance);
    }
}

void EditorCamera::Rotate(const Vector2 &delta)
{
    Vector3 offset = m_position - m_focus;
    
    Vector3 lookDir = offset.Normalized();
    Vector3 camRight = Vector3::Cross(lookDir, Vector3::UP);
    
    Quaternion xRot = Quaternion::FromAxisAngle(Vector3::UP, -delta.x);
    Quaternion yRot = Quaternion::FromAxisAngle(camRight, delta.y);
    
    if ((Vector3::Dot(lookDir, Vector3::UP) > 0.98f && delta.y > 0)
        || (Vector3::Dot(lookDir, Vector3::DOWN) > 0.98f && delta.y < 0))
        yRot.SetIdentity();
    
    Quaternion rotation = xRot * yRot;

    m_position = m_focus + rotation * offset;
    m_direction = (m_focus - m_position).Normalized();
}

void EditorCamera::Pan(const Vector2 &delta)
{
    Vector3 offset = m_position - m_focus;
    
    Vector3 lookDir = (m_position - m_focus).Normalize();
    Vector3 camRight = Vector3::Cross(lookDir, Vector3::UP).Normalize();
    Vector3 camUp = Vector3::Cross(lookDir, camRight).Normalize();

    m_focus += camRight * delta.x;
    m_focus -= camUp * delta.y;
    m_position = m_focus + offset;
}

void EditorCamera::Zoom(float delta)
{
    Vector3 offset = m_position - m_focus;
    float zoom = Max(offset.Magnitude() + delta, 0.1f);
    m_position = m_focus + offset.Normalized() * zoom;
}
