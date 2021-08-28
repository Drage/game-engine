
#include "Particle.h"
#include "GameTime.h"
#include "MathUtils.h"

using namespace DrageEngine;

Particle::Particle(
    const Vector3 &position,
    float size,
    float rotation,
    float lifeTime,
    const Vector3 &velocity,
    const Color &color,
    float angularVelocity,
    float gravity)
{
    m_position = position;
    m_rotation = rotation;
    m_scale = size;
    
    m_birthTime = Time::RunTime();
    m_deathTime = m_birthTime + lifeTime;

    m_velocity = velocity;
    m_angularVelocity = angularVelocity;
    m_gravity = gravity;
    
    m_color = color;
    m_alpha = 1;

    m_animateScale = false;
    m_animateColor = false;
    m_animateAlpha = false;
}

void Particle::Update()
{
    int currentTime = Time::RunTime();
    if (currentTime > m_deathTime)
    {
        m_destroyed = true;
    }
    else
    {
        float timePercent = (currentTime - m_birthTime) / (m_deathTime - m_birthTime);

         if (m_animateScale)
             m_scale = Animate(m_startScale, m_normalScale, m_endScale, m_scaleInPercent, m_scaleOutPercent, timePercent);
        
        if (m_animateColor)
            m_color = Animate(m_startColor, m_normalColor, m_endColor, m_colorInPercent, m_colorOutPercent, timePercent);

        if (m_animateAlpha)
            m_alpha = Animate(m_startAlpha, m_normalAlpha, m_endAlpha, m_alphaInPercent, m_alphaOutPercent, timePercent);

        float deltaTime = Time::DeltaTime();
        m_velocity.y -= m_gravity * deltaTime;
        m_position += m_velocity * deltaTime;
        m_rotation += m_angularVelocity * deltaTime;
    }
}

void Particle::AnimateScale(float start, float end, float inPercent, float outPercent)
{
    m_animateScale = true;
    m_startScale = start;
    m_normalScale = m_scale;
    m_endScale = end;
    m_scaleInPercent = inPercent;
    m_scaleOutPercent = outPercent;
    m_scale = m_startScale;
}

void Particle::AnimateColor(const Color &start, const Color &end, float inPercent, float outPercent)
{
    m_animateColor = true;
    m_startColor = start;
    m_normalColor = m_color;
    m_endColor = end;
    m_colorInPercent = inPercent;
    m_colorOutPercent = outPercent;
    m_color = m_startColor;
}

void Particle::AnimateAlpha(float start, float end, float inPercent, float outPercent)
{
    m_animateAlpha = true;
    m_startAlpha = start;
    m_normalAlpha = m_alpha;
    m_endAlpha = end;
    m_alphaInPercent = inPercent;
    m_alphaOutPercent = outPercent;
    m_alpha = m_startAlpha;
}
