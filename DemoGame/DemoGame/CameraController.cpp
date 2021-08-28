
#include "CameraController.h"

void CameraController::Init(const ParamList &params)
{
    m_rotateSensitivity = params.Get<float>("rotateSensitivity", 0.8f);
    m_panSensitivity = params.Get<float>("panSensitivity", 0.2f);
    m_zoomSensitivity = params.Get<float>("zoomSensitivity", 0.5f);
    m_focus = params.Get<Vector3>("focus");
}

void CameraController::Update()
{
    Input *input = app->input;
    Vector2 mouseDelta = input->GetMouseDelta();
    
    float distance = (transform->position - m_focus).Magnitude();
    
    if (input->GetMouseButton(Mouse::LEFT))
    {
        if (input->GetKey(Key::LALT) || input->GetKey(Key::RALT))
            Rotate(mouseDelta * m_rotateSensitivity * Time::DeltaTime());
        else
            Pan(mouseDelta * m_panSensitivity * Time::DeltaTime() * distance);
    }
    else if (input->GetMouseButton(Mouse::MIDDLE))
    {
        Zoom(mouseDelta.y * m_zoomSensitivity * Time::DeltaTime() * distance);
    }
}

void CameraController::Rotate(const Vector2 &delta)
{
    Vector3 offset = transform->position - m_focus;
    
    Vector3 lookDir = offset.Normalized();
    Vector3 camRight = Vector3::Cross(lookDir, Vector3::UP);
    
    Quaternion xRot = Quaternion::FromAxisAngle(Vector3::UP, -delta.x);
    Quaternion yRot = Quaternion::FromAxisAngle(camRight, delta.y);
    
    if ((Vector3::Dot(lookDir, Vector3::UP) > 0.98f && delta.y > 0)
        || (Vector3::Dot(lookDir, Vector3::DOWN) > 0.98f && delta.y < 0))
        yRot.SetIdentity();
    
    Quaternion rotation = xRot * yRot;
    
    transform->position = m_focus + rotation * offset;
    transform->rotation = rotation * transform->rotation;
}

void CameraController::Pan(const Vector2 &delta)
{
    Vector3 offset = transform->position - m_focus;
    
    m_focus += transform->Right() * delta.x;
    m_focus += transform->Up() * delta.y;
    
    transform->position = m_focus + offset;
}

void CameraController::Zoom(float delta)
{
    Vector3 offset = transform->position - m_focus;
    float zoom = Max(offset.Magnitude() + delta, 0.1f);
    transform->position = m_focus + offset.Normalized() * zoom;
}
