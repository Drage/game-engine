
#include "PlaneComponent.h"
#include "Application.h"
#include "Primitives.h"
#include "Material.h"

using namespace DrageEngine;

void PlaneComponent::Init(const ParamList &params)
{
    std::string materialName = params.Get<std::string>("material");
    Material *material = app->assets->GetMaterial(materialName);
    
    plane = new Renderable(Primitive::Plane(), material, entity);
}

PlaneComponent::~PlaneComponent()
{
    delete plane;
}
