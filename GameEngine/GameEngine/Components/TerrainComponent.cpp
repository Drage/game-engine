
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
    
    disableDepthWrite = params.Get<bool>("disableDepthWrite");
    
    renderable = new Renderable(GetMesh(), material, entity, disableDepthWrite ? RenderOption::NO_DEPTH_WRITE : 0);
}

TerrainComponent::~TerrainComponent()
{
    delete renderable;
}
