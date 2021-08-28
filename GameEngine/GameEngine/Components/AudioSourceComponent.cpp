
#include "AudioSourceComponent.h"
#include "StringUtils.h"
#include "Application.h"

using namespace DrageEngine;

void AudioSourceComponent::Init(const ParamList &params)
{
    std::string clipName = params.Get<std::string>("clip");
    if (!String::IsNullOrEmpty(clipName))
    {
        m_clip = app->assets->GetAudio(clipName);
        SetClip(m_clip);
    }
}

void AudioSourceComponent::Update()
{
    SetPosition(transform->position);
    SetVelocity(transform->position - m_lastPosition);
    
    m_lastPosition = transform->position;
}
