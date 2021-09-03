
#include "SphereComponent.h"
#include "Application.h"
#include "Material.h"
#include "Primitives.h"

using namespace DrageEngine;

void SphereComponent::Init(const ParamList &params)
{
    std::string materialName = params.Get<std::string>("material");
    Material *material = app->assets->GetMaterial(materialName);
    
    sphere = new Renderable(Primitive::Sphere(), material, entity);
}

SphereComponent::~SphereComponent()
{
    delete sphere;
}
