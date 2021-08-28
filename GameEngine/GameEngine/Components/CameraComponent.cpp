
#include "CameraComponent.h"
#include "Application.h"

using namespace DrageEngine;

void CameraComponent::Init(const ParamList &params)
{   
    m_fov = params.Get<float>("fov", 60);
    m_nearClip = params.Get<float>("nearClip", 0.1f);
    m_farClip = params.Get<float>("farClip", 1000);
}

void CameraComponent::Start()
{
    app->renderer->SetActiveCamera(this);
}

void CameraComponent::Update()
{
    m_position = transform->position;
    m_direction = transform->Forward();
}
