
#include <vector>
#include "EditorCamera.h"
#include "Input.h"
#include "Application.h"
#include "MathUtils.h"

using namespace DrageEngine;

EditorCamera::EditorCamera()
{
    rotateSensitivity = 0.4f;
    orbitSensitivity = 0.8f;
    panSensitivity = 0.2f;
    zoomSensitivity = 0.5f;
    position = Vector3(0, 5, 0);
    focus = Vector3(0, 5, 10);
    direction = (focus - position).Normalized();
    arrowKeyMoveSpeedSlow = 50.0f;
    arrowKeyMoveSpeedFast = 200.0f;
    moveToTarget = false;
    targetTransitionSpeed = 0.3f;
    targetPosition = Vector3(0);
    targetFocus = Vector3(0);
}

void EditorCamera::Update()
{
    if (moveToTarget)
    {
        focus = Vector3::Lerp(focus, targetFocus, targetTransitionSpeed);
        direction = (focus - position).Normalized();

        position = Vector3::Lerp(position, targetPosition, targetTransitionSpeed);

        if (Vector3::Distance(focus, targetFocus) <= targetTransitionSpeed && Vector3::Distance(position, targetPosition) <= targetTransitionSpeed)
            moveToTarget = false;
        else
            return;
    }

    Input *input = app->input;
    Vector2 mouseDelta = input->GetMouseDelta();

    float distance = (position - focus).Magnitude();

    if (input->GetMouseButton(Mouse::LEFT))
    {
        if (input->GetKey(Key::LALT) || input->GetKey(Key::RALT))
        {
            if (input->GetKey(Key::LSHIFT))
                Rotate(mouseDelta * rotateSensitivity * Time::DeltaTime());
            else
                Orbit(mouseDelta * orbitSensitivity * Time::DeltaTime());
        }
        else if (input->GetKey(Key::LCOMMAND) || input->GetKey(Key::RCOMMAND))
            Zoom(mouseDelta.y * zoomSensitivity * Time::DeltaTime() * distance);
        else if (input->GetKey(Key::LCTRL) || input->GetKey(Key::RCTRL))
            Pan(mouseDelta * panSensitivity * Time::DeltaTime() * distance);
    }
    else if (input->GetMouseButton(Mouse::MIDDLE))
    {
        Zoom(mouseDelta.y * zoomSensitivity * Time::DeltaTime() * distance);
    }

    float arrowMoveSpeed = input->GetKey(Key::LSHIFT) ? arrowKeyMoveSpeedFast : arrowKeyMoveSpeedSlow;
    Vector3 arrowMoveDirection;
    Vector3 strafeDirection = Vector3::Cross(direction, Vector3::UP);
    if (input->GetKey(Key::UP)) arrowMoveDirection += direction;
    if (input->GetKey(Key::DOWN)) arrowMoveDirection -= direction;
    if (input->GetKey(Key::LEFT)) arrowMoveDirection -= strafeDirection;
    if (input->GetKey(Key::RIGHT)) arrowMoveDirection += strafeDirection;
    Vector3 delta = arrowMoveDirection * arrowMoveSpeed * Time::DeltaTime();
    position += delta;
    focus += delta;
}

void EditorCamera::Rotate(const Vector2 &delta)
{
    Vector3 camRight = Vector3::Cross(Vector3::UP, direction);

    Quaternion xRot = Quaternion::FromAxisAngle(Vector3::UP, -delta.x);
    Quaternion yRot = Quaternion::FromAxisAngle(camRight, delta.y);

    Quaternion rotation = xRot * yRot;

    float focusDistance = Vector3::Distance(position, focus);
    direction = rotation * direction;
    focus = position + direction * focusDistance;
}

void EditorCamera::Orbit(const Vector2 &delta)
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

void EditorCamera::SetFocus(const Vector3 &focus, float distance)
{
    moveToTarget = true;
    targetFocus = focus;
    Vector3 direction = (position - focus).Normalized();
    targetPosition = focus + direction * distance;
}
