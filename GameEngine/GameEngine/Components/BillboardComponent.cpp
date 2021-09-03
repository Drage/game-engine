
#include "BillboardComponent.h"
#include "Application.h"
#include "Primitives.h"
#include "Material.h"

using namespace DrageEngine;

void BillboardComponent::Init(const ParamList &params)
{
    std::string materialName = params.Get<std::string>("material");
    Material *material = app->assets->GetMaterial(materialName);
    
    billboard = new Renderable(Primitive::Billboard(), material, entity);
}

BillboardComponent::~BillboardComponent()
{
    delete billboard;
}
