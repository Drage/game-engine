
#include "Camera3rdPerson.h"

void Camera3rdPerson::Init(const ParamList &params)
{
    height = params.Get<float>("height", 4.0f);
    distanceBehind = params.Get<float>("distanceBehind", 8.0f);
    lookAhead = params.Get<float>("lookAhead", 15.0f);
    lookYOffset = params.Get<float>("lookYOffset", 1.0f);
    moveSmoothing = params.Get<float>("moveSmoothing", 24);
    lookSmoothing = params.Get<float>("lookSmoothing", 12);
    targetPath = params.Get<std::string>("target");
}

void Camera3rdPerson::Start()
{
    if (!String::IsNullOrEmpty(targetPath))
    {
        target = app->GetActiveScene()->Find(targetPath);
        if (target)
        {
            lookAt = target->transform.position;
            lastLookY = lookAt.y;
        }
    }
}

void Camera3rdPerson::Update()
{
    if (target)
    {
        Vector3 targetPosition = target->transform.position;
        
        // Transform forward vector to target's orientation
        Vector3 forwards = target->transform.rotation * Vector3::FORWARD;
        
        // Update camera position
        Vector3 targetCamPosition = targetPosition - forwards * distanceBehind;
        targetCamPosition.y += height;
        transform->position = Vector3::Lerp(transform->position, targetCamPosition, moveSmoothing * Time::DeltaTime());
        
        // Update look position
        Vector3 targetLookPosition = targetPosition + forwards * lookAhead + Vector3::UP * lookYOffset;
        lastLookY = lookAt.y;
        lookAt = Vector3::Lerp(lookAt, targetLookPosition, lookSmoothing * Time::DeltaTime());
        lookAt.y = lastLookY + (lookAt.y - lastLookY) * lookSmoothing / 4 * Time::DeltaTime();
        
        // Update camera rotation
        transform->rotation = Quaternion::FromLookDirection(lookAt - transform->position);
    }
}
