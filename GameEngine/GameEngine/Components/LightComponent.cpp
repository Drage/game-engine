
#include "LightComponent.h"
#include "Application.h"
#include "Renderer.h"
#include "Debug.h"

using namespace DrageEngine;

void LightComponent::Init(const ParamList &params)
{
    type = (Light::Type)params.Get<int>("type");
    color = params.Get<Color>("color");
    ambientCoefficient = params.Get<float>("ambient");
    attenuation = params.Get<float>("attenuation");
    coneAngle = params.Get<float>("angle");
    app->renderer->AddLight(this);
    Update();
}

LightComponent::~LightComponent()
{
    app->renderer->RemoveLight(this);
}

void LightComponent::Update()
{
    if (type == DIRECTIONAL)
        position = Vector4(transform->Forward(), 0);
    else
        position = Vector4(transform->position, 1);

    if (type == SPOTLIGHT)
        coneDirection = transform->Forward();
}
