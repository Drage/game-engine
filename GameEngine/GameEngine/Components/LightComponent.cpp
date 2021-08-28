
#include "LightComponent.h"
#include "Application.h"
#include "Renderer.h"
#include "Debug.h"

using namespace DrageEngine;

void LightComponent::Init(const ParamList &params)
{
    m_type = (Light::Type)params.Get<int>("type");
    m_color = params.Get<Color>("color");
    m_ambientCoefficient = params.Get<float>("ambient");
    m_attenuation = params.Get<float>("attenuation");
    m_coneAngle = params.Get<float>("angle");
    app->renderer->AddLight(this);
}

LightComponent::~LightComponent()
{
    app->renderer->RemoveLight(this);
}

void LightComponent::Update()
{
    if (m_type == DIRECTIONAL)
        m_position = Vector4(transform->Forward(), 0);
    else
        m_position = Vector4(transform->position, 1);

    if (m_type == SPOTLIGHT)
        m_coneDirection = transform->Forward();
}
