
#include "SkyboxComponent.h"
#include "Application.h"
#include "Material.h"
#include "Primitives.h"

using namespace DrageEngine;

void SkyboxComponent::Init(const ParamList &params)
{
    std::string materialName = params.Get<std::string>("material");
    Material *material = app->assets->GetMaterial(materialName);
    
    skybox = new Renderable(Primitive::Cube(), material, entity, RenderOption::RENDER_BACKFACE | RenderOption::NO_DEPTH_WRITE);
}

SkyboxComponent::~SkyboxComponent()
{
    delete skybox;
}
