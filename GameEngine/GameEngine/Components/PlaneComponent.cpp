
#include "PlaneComponent.h"
#include "Application.h"
#include "Primitives.h"

using namespace DrageEngine;

void PlaneComponent::Init(const ParamList &params)
{
    std::string materialName = params.Get<std::string>("material");
    m_material = app->assets->GetMaterial(materialName);
}

void PlaneComponent::Render(Renderer *renderer, const Transform *transform) const
{
    renderer->Render(&m_mesh, transform, m_material);
}
