
#include "Cloud.h"

void Cloud::Init(const ParamList &params)
{
    size = params.Get<float>("size");
    transform->Scale(Vector3(size));
    
    yPosition = params.Get<float>("yPosition");
    orbitRadius = params.Get<float>("orbitRadius");
    angle = DegToRad(Random::Float(0.0f, 360.0f));
    float x = orbitRadius * cos(angle);
    float z = orbitRadius * sin(angle);
    transform->Translate(Vector3(x, yPosition, z));
    
    speed = params.Get<float>("speed");
    
    std::string materialName = params.Get<std::string>("material");
    Material *material = app->assets->GetMaterial(materialName);
    renderable = new Renderable(Primitive::Billboard(), material, entity, RenderOption::FRUSTUM_CULL_SPHERE);
}

void Cloud::Update()
{
    Vector3 cameraPosition = app->renderer->GetActiveCamera()->GetPosition();
    angle += DegToRad(speed * Time::DeltaTime());
    float x = orbitRadius * cos(angle) + cameraPosition.x;
    float z = orbitRadius * sin(angle) + cameraPosition.z;
    transform->Translate(Vector3(x, yPosition, z));
}

Cloud::~Cloud()
{
    delete renderable;
}
