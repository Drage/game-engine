
#include "Clouds.h"

void Clouds::Init(const ParamList &params)
{
    m_numClouds = params.Get<int>("count");
    m_orbitRadius = params.Get<float>("orbitRadius");
    m_minYPos = params.Get<float>("minYPosition");
    m_maxYPos = params.Get<float>("maxYPosition");
    m_minSpeed = params.Get<float>("minSpeed");
    m_maxSpeed = params.Get<float>("maxSpeed");
    m_minSize = params.Get<float>("minSize");
    m_maxSize = params.Get<float>("maxSize");
    
    m_clouds = new Cloud[m_numClouds];
    
    for (int i = 0; i < m_numClouds; i++)
    {
        std::string matName = "Cloud0";
        matName.append(1, (char)(Random::Int(1, NUM_CLOUD_TEXTURES) + (int)'0'));
        matName.append(".material");
        m_clouds[i].material = app->assets->GetMaterial(matName);

        m_clouds[i].angle = DegToRad(Random::Float(0.0f, 360.0f));
        m_clouds[i].position.y = Random::Float(m_minYPos, m_maxYPos);
        m_clouds[i].size = Random::Float(m_minSize, m_maxSize);
        m_clouds[i].speed = Random::Float(m_minSpeed, m_maxSpeed);
    }
}

Clouds::~Clouds()
{
    delete[] m_clouds;
}

void Clouds::Update()
{
    Vector3 cameraPosition = app->renderer->GetActiveCamera()->GetPosition();
    
    for (int i = 0; i < m_numClouds; i++)
    {
        m_clouds[i].angle += DegToRad(m_clouds[i].speed * Time::DeltaTime());
        m_clouds[i].position.x = m_orbitRadius * cos(m_clouds[i].angle) + cameraPosition.x;
        m_clouds[i].position.z = m_orbitRadius * sin(m_clouds[i].angle) + cameraPosition.z;
    }
}

void Clouds::Render(Renderer *renderer, const Transform *transform) const
{
    for (int i = 0; i < m_numClouds; i++)
    {
        Transform cloudTransform = *transform;
        cloudTransform.Scale(Vector3(m_clouds[i].size));
        cloudTransform.Translate(m_clouds[i].position);
        
        renderer->Render(&m_billboardQuad, &cloudTransform, m_clouds[i].material, RenderOption::NO_DEPTH_WRITE);
    }
}
