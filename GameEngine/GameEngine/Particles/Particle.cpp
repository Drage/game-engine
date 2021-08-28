
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
    this->position = position;
    this->rotation = rotation;
    this->scale = size;
    
    this->birthTime = Time::RunTime();
    this->deathTime = birthTime + lifeTime;

    this->velocity = velocity;
    this->angularVelocity = angularVelocity;
    this->gravity = gravity;
    
    this->color = color;
    this->alpha = 1;

    this->animateScale = false;
    this->animateColor = false;
    this->animateAlpha = false;
}

void Particle::Update()
{
    int currentTime = Time::RunTime();
    if (currentTime > deathTime)
    {
        destroyed = true;
    }
    else
    {
        float timePercent = (currentTime - birthTime) / (deathTime - birthTime);

         if (animateScale)
             scale = Animate(startScale, normalScale, endScale, scaleInPercent, scaleOutPercent, timePercent);
        
        if (animateColor)
            color = Animate(startColor, normalColor, endColor, colorInPercent, colorOutPercent, timePercent);

        if (animateAlpha)
            alpha = Animate(startAlpha, normalAlpha, endAlpha, alphaInPercent, alphaOutPercent, timePercent);

        float deltaTime = Time::DeltaTime();
        velocity.y -= gravity * deltaTime;
        position += velocity * deltaTime;
        rotation += angularVelocity * deltaTime;
    }
}

void Particle::AnimateScale(float start, float end, float inPercent, float outPercent)
{
    animateScale = true;
    startScale = start;
    normalScale = scale;
    endScale = end;
    scaleInPercent = inPercent;
    scaleOutPercent = outPercent;
    scale = startScale;
}

void Particle::AnimateColor(const Color &start, const Color &end, float inPercent, float outPercent)
{
    animateColor = true;
    startColor = start;
    normalColor = color;
    endColor = end;
    colorInPercent = inPercent;
    colorOutPercent = outPercent;
    color = startColor;
}

void Particle::AnimateAlpha(float start, float end, float inPercent, float outPercent)
{
    animateAlpha = true;
    startAlpha = start;
    normalAlpha = alpha;
    endAlpha = end;
    alphaInPercent = inPercent;
    alphaOutPercent = outPercent;
    alpha = startAlpha;
}
