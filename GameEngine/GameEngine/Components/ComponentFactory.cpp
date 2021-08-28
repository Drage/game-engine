
#include "ComponentFactory.h"
#include "MeshRendererComponent.h"
#include "LightComponent.h"
#include "CameraComponent.h"
#include "CubeComponent.h"
#include "PlaneComponent.h"
#include "AudioListenerComponent.h"
#include "AudioSourceComponent.h"
#include "SkyboxComponent.h"
#include "TerrainComponent.h"
#include "BillboardComponent.h"

using namespace DrageEngine;

ComponentFactory::ComponentFactory()
{
    factory.Register<MeshRendererComponent>("MeshRenderer");
    factory.Register<LightComponent>("Light");
    factory.Register<CameraComponent>("Camera");
    factory.Register<CubeComponent>("Cube");
    factory.Register<PlaneComponent>("Plane");
    factory.Register<AudioListenerComponent>("AudioListener");
    factory.Register<AudioSourceComponent>("AudioSource");
    factory.Register<SkyboxComponent>("Skybox");
    factory.Register<TerrainComponent>("Terrain");
    factory.Register<BillboardComponent>("Billboard");
}

Component* ComponentFactory::Create(const std::string &type)
{
    return factory.Create(type);
}

bool ComponentFactory::IsSupported(const std::string &type) const
{
    return factory.IsSupported(type);
}
