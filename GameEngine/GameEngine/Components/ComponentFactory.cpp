
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
    m_factory.Register<MeshRendererComponent>("MeshRenderer");
    m_factory.Register<LightComponent>("Light");
    m_factory.Register<CameraComponent>("Camera");
    m_factory.Register<CubeComponent>("Cube");
    m_factory.Register<PlaneComponent>("Plane");
    m_factory.Register<AudioListenerComponent>("AudioListener");
    m_factory.Register<AudioSourceComponent>("AudioSource");
    m_factory.Register<SkyboxComponent>("Skybox");
    m_factory.Register<TerrainComponent>("Terrain");
    m_factory.Register<BillboardComponent>("Billboard");
}

Component* ComponentFactory::Create(const std::string &type)
{
    return m_factory.Create(type);
}

bool ComponentFactory::IsSupported(const std::string &type) const
{
    return m_factory.IsSupported(type);
}
