
#include "AudioSourceComponent.h"
#include "StringUtils.h"
#include "Application.h"

using namespace DrageEngine;

void AudioSourceComponent::Init(const ParamList &params)
{
    std::string clipName = params.Get<std::string>("clip");
    if (!String::IsNullOrEmpty(clipName))
    {
        clip = app->assets->GetAudio(clipName);
        SetClip(clip);
    }
}

void AudioSourceComponent::Update()
{
    SetPosition(transform->position);
    SetVelocity(transform->position - lastPosition);
    
    lastPosition = transform->position;
}
