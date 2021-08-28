
#include "Camera3rdPerson.h"

void Camera3rdPerson::Init(const ParamList &params)
{
    m_height = params.Get<float>("height", 4.0f);
    m_distanceBehind = params.Get<float>("distanceBehind", 8.0f);
    m_lookAhead = params.Get<float>("lookAhead", 15.0f);
    m_lookYOffset = params.Get<float>("lookYOffset", 1.0f);
    m_moveSmoothing = params.Get<float>("moveSmoothing", 24);
    m_lookSmoothing = params.Get<float>("lookSmoothing", 12);
    m_targetPath = params.Get<std::string>("target");
}

void Camera3rdPerson::Start()
{
    if (!String::IsNullOrEmpty(m_targetPath))
    {
        m_target = app->GetActiveScene()->Find(m_targetPath);
        if (m_target)
        {
            m_lookAt = m_target->transform.position;
            m_lastLookY = m_lookAt.y;
        }
    }
}

void Camera3rdPerson::Update()
{
    if (m_target)
    {
        Vector3 targetPosition = m_target->transform.position;
        
        // Transform forward vector to target's orientation
        Vector3 forwards = m_target->transform.rotation * Vector3::FORWARD;
        
        // Update camera position
        Vector3 targetCamPosition = targetPosition - forwards * m_distanceBehind;
        targetCamPosition.y += m_height;
        transform->position = Vector3::Lerp(transform->position, targetCamPosition, m_moveSmoothing * Time::DeltaTime());
        
        // Update look position
        Vector3 targetLookPosition = targetPosition + forwards * m_lookAhead + Vector3::UP * m_lookYOffset;
        m_lastLookY = m_lookAt.y;
        m_lookAt = Vector3::Lerp(m_lookAt, targetLookPosition, m_lookSmoothing * Time::DeltaTime());
        m_lookAt.y = m_lastLookY + (m_lookAt.y - m_lastLookY) * m_lookSmoothing / 4 * Time::DeltaTime();
        
        // Update camera rotation
        transform->rotation = Quaternion::FromLookDirection(m_lookAt - transform->position);
    }
}
