
#include "SphereComponent.h"
#include "Application.h"
#include "Primitives.h"

using namespace DrageEngine;

void SphereComponent::Init(const ParamList &params)
{
    std::string materialName = params.Get<std::string>("material");
    material = app->assets->GetMaterial(materialName);
}

void SphereComponent::Render(Renderer *renderer, const Transform *transform) const
{
    renderer->Render(&mesh, transform, material);
}
