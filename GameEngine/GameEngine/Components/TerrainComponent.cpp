
#include "TerrainComponent.h"
#include "Application.h"

using namespace DrageEngine;

void TerrainComponent::Init(const ParamList &params)
{
    std::string heightmap = params.Get<std::string>("heightmap");
    float maxHeight = params.Get<float>("maxHeight", 10);
    float resolution = params.Get<float>("resolution", 1);
    
    Load(heightmap, maxHeight, resolution);
    
    std::string materialName = params.Get<std::string>("material");
    material = app->assets->GetMaterial(materialName);
    
    float renderPriority = params.Get<float>("renderPriority");
    
    renderable = new Renderable(GetMesh(), material, entity);
    renderable->SetDepthOrder(renderPriority);
}

TerrainComponent::~TerrainComponent()
{
    delete renderable;
}
