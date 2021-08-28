
#include "EditorCamera.h"
#include "Input.h"
#include "Application.h"
#include "MathUtils.h"

using namespace DrageEngine;

EditorCamera::EditorCamera()
{
    rotateSensitivity = 0.8f;
    panSensitivity = 0.2f;
    zoomSensitivity = 0.5f;
    focus = Vector3(0, 0, 0);
}

void EditorCamera::Update()
{
    Input *input = app->input;
    Vector2 mouseDelta = input->GetMouseDelta();
    
    float distance = (position - focus).Magnitude();
    
    if (input->GetMouseButton(Mouse::LEFT))
    {
        if (input->GetKey(Key::LALT) || input->GetKey(Key::RALT))
            Rotate(mouseDelta * rotateSensitivity * Time::DeltaTime());
        else if (input->GetKey(Key::LCOMMAND) || input->GetKey(Key::RCOMMAND))
            Zoom(mouseDelta.y * zoomSensitivity * Time::DeltaTime() * distance);
        else
            Pan(mouseDelta * panSensitivity * Time::DeltaTime() * distance);
    }
    else if (input->GetMouseButton(Mouse::MIDDLE))
    {
        Zoom(mouseDelta.y * zoomSensitivity * Time::DeltaTime() * distance);
    }
}

void EditorCamera::Rotate(const Vector2 &delta)
{
    Vector3 offset = position - focus;
    
    Vector3 lookDir = offset.Normalized();
    Vector3 camRight = Vector3::Cross(lookDir, Vector3::UP);
    
    Quaternion xRot = Quaternion::FromAxisAngle(Vector3::UP, -delta.x);
    Quaternion yRot = Quaternion::FromAxisAngle(camRight, delta.y);
    
    if ((Vector3::Dot(lookDir, Vector3::UP) > 0.98f && delta.y > 0)
        || (Vector3::Dot(lookDir, Vector3::DOWN) > 0.98f && delta.y < 0))
        yRot.SetIdentity();
    
    Quaternion rotation = xRot * yRot;

    position = focus + rotation * offset;
    direction = (focus - position).Normalized();
}

void EditorCamera::Pan(const Vector2 &delta)
{
    Vector3 offset = position - focus;
    
    Vector3 lookDir = (position - focus).Normalize();
    Vector3 camRight = Vector3::Cross(lookDir, Vector3::UP).Normalize();
    Vector3 camUp = Vector3::Cross(lookDir, camRight).Normalize();

    focus += camRight * delta.x;
    focus -= camUp * delta.y;
    position = focus + offset;
}

void EditorCamera::Zoom(float delta)
{
    Vector3 offset = position - focus;
    float zoom = Max(offset.Magnitude() + delta, 0.1f);
    position = focus + offset.Normalized() * zoom;
}
