
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
    m_material = app->assets->GetMaterial(materialName);
    
    m_disableDepthWrite = params.Get<bool>("disableDepthWrite");
}

void TerrainComponent::Render(Renderer *renderer, const Transform *transform) const
{
    renderer->Render(this, transform, m_material, m_disableDepthWrite ? RenderOption::NO_DEPTH_WRITE : 0);
}
