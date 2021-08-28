
#include <OpenAL/al.h>
#include "AudioListener.h"

using namespace DrageEngine;

void AudioListener::SetPosition(const Vector3 &position)
{
    alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void AudioListener::SetVelocity(const Vector3 &velocity)
{
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void AudioListener::SetOrientation(const Quaternion &rotation)
{
    Vector3 fwd = rotation * Vector3::FORWARD;
    Vector3 up  = rotation * Vector3::UP;
    float orientation[] = { fwd.x, fwd.y, fwd.z, up.x, up.y, up.z };
    alListenerfv(AL_ORIENTATION, orientation);
}
