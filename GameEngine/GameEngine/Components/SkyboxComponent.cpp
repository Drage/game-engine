
#include "SkyboxComponent.h"
#include "Application.h"

using namespace DrageEngine;

void SkyboxComponent::Init(const ParamList &params)
{
    std::string materialName = params.Get<std::string>("material");
    material = app->assets->GetMaterial(materialName);
}

void SkyboxComponent::Render(Renderer *renderer, const Transform *transform) const
{
    renderer->Render(&mesh, transform, material, RenderOption::RENDER_BACKFACE);
}
