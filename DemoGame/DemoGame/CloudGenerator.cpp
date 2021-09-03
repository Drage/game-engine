
#include "CloudGenerator.h"
#include "Cloud.h"

void CloudGenerator::Init(const ParamList &params)
{
    numClouds = params.Get<int>("count");
    orbitRadius = params.Get<float>("orbitRadius");
    minYPos = params.Get<float>("minYPosition");
    maxYPos = params.Get<float>("maxYPosition");
    minSpeed = params.Get<float>("minSpeed");
    maxSpeed = params.Get<float>("maxSpeed");
    minSize = params.Get<float>("minSize");
    maxSize = params.Get<float>("maxSize");
    
    for (int i = 0; i < numClouds; i++)
    {
        Entity *child = new Entity();
        
        ParamList params;
        params.Set("orbitRadius", orbitRadius);
        params.Set("yPosition", Random::Float(minYPos, maxYPos));
        params.Set("size", Random::Float(minSize, maxSize));
        params.Set("speed", Random::Float(minSpeed, maxSpeed));
        
        std::string material = "Cloud0";
        material.append(1, (char)(Random::Int(1, NUM_CLOUD_TEXTURES) + (int)'0'));
        material.append(".material");
        params.Set("material", material);
        
        Cloud *cloud = new Cloud();
        child->AddComponent(cloud);
        cloud->Init(params);
        
        entity->AddChild(child);
    }
}
