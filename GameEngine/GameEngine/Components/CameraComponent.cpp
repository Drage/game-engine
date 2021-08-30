
#include "CameraComponent.h"
#include "Application.h"

using namespace DrageEngine;

void CameraComponent::Init(const ParamList &params)
{   
    fov = params.Get<float>("fov", 60);
    nearClip = params.Get<float>("nearClip", 0.1f);
    farClip = params.Get<float>("farClip", 10000);
}

void CameraComponent::Start()
{
    app->renderer->SetActiveCamera(this);
}

void CameraComponent::Update()
{
    position = transform->position;
    direction = transform->Forward();
}
