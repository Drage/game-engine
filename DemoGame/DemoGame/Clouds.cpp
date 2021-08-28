
#include "Clouds.h"

void Clouds::Init(const ParamList &params)
{
    numClouds = params.Get<int>("count");
    orbitRadius = params.Get<float>("orbitRadius");
    minYPos = params.Get<float>("minYPosition");
    maxYPos = params.Get<float>("maxYPosition");
    minSpeed = params.Get<float>("minSpeed");
    maxSpeed = params.Get<float>("maxSpeed");
    minSize = params.Get<float>("minSize");
    maxSize = params.Get<float>("maxSize");
    
    clouds = new Cloud[numClouds];
    
    for (int i = 0; i < numClouds; i++)
    {
        std::string matName = "Cloud0";
        matName.append(1, (char)(Random::Int(1, NUM_CLOUD_TEXTURES) + (int)'0'));
        matName.append(".material");
        clouds[i].material = app->assets->GetMaterial(matName);

        clouds[i].angle = DegToRad(Random::Float(0.0f, 360.0f));
        clouds[i].position.y = Random::Float(minYPos, maxYPos);
        clouds[i].size = Random::Float(minSize, maxSize);
        clouds[i].speed = Random::Float(minSpeed, maxSpeed);
    }
}

Clouds::~Clouds()
{
    delete[] clouds;
}

void Clouds::Update()
{
    Vector3 cameraPosition = app->renderer->GetActiveCamera()->GetPosition();
    
    for (int i = 0; i < numClouds; i++)
    {
        clouds[i].angle += DegToRad(clouds[i].speed * Time::DeltaTime());
        clouds[i].position.x = orbitRadius * cos(clouds[i].angle) + cameraPosition.x;
        clouds[i].position.z = orbitRadius * sin(clouds[i].angle) + cameraPosition.z;
    }
}

void Clouds::Render(Renderer *renderer, const Transform *transform) const
{
    for (int i = 0; i < numClouds; i++)
    {
        Transform cloudTransform = *transform;
        cloudTransform.Scale(Vector3(clouds[i].size));
        cloudTransform.Translate(clouds[i].position);
        
        renderer->Render(&billboardQuad, &cloudTransform, clouds[i].material, RenderOption::NO_DEPTH_WRITE);
    }
}
