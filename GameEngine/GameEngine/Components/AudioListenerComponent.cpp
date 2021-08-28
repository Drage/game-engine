
#include "AudioListenerComponent.h"

using namespace DrageEngine;

void AudioListenerComponent::Update()
{
    SetPosition(transform->position);
    SetOrientation(transform->rotation);
    SetVelocity(transform->position - lastPosition);
    
    lastPosition = transform->position;
}
