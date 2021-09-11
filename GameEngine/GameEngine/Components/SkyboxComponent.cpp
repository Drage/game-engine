
#include "SkyboxComponent.h"
#include "Application.h"
#include "Material.h"
#include "Primitives.h"

using namespace DrageEngine;

void SkyboxComponent::Init(const ParamList &params)
{
    std::string materialName = params.Get<std::string>("material");
    Material *material = app->assets->GetMaterial(materialName);
    
    unsigned renderOptions = RenderOption::RENDER_BACKFACE | RenderOption::NO_DEPTH_WRITE | RenderOption::NO_FRUSTUM_CULL | RenderOption::NON_SELECTABLE;
    skybox = new Renderable(Primitive::Cube(), material, entity, renderOptions);
}

SkyboxComponent::~SkyboxComponent()
{
    delete skybox;
}
