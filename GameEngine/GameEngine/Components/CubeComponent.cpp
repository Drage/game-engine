
#include "CubeComponent.h"
#include "Application.h"
#include "Primitives.h"
#include "Material.h"

using namespace DrageEngine;

void CubeComponent::Init(const ParamList &params)
{
    std::string materialName = params.Get<std::string>("material");
    Material *material = app->assets->GetMaterial(materialName);
    
    cube = new Renderable(Primitive::Cube(), material, entity);
}

CubeComponent::~CubeComponent()
{
    delete cube;
}
