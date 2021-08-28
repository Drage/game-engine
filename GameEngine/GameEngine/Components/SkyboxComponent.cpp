
#include "SkyboxComponent.h"
#include "Application.h"

using namespace DrageEngine;

void SkyboxComponent::Init(const ParamList &params)
{
    std::string materialName = params.Get<std::string>("material");
    m_material = app->assets->GetMaterial(materialName);
}

void SkyboxComponent::Render(Renderer *renderer, const Transform *transform) const
{
    renderer->Render(&m_mesh, transform, m_material, RenderOption::RENDER_BACKFACE);
}
