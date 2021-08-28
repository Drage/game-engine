
#include <OpenAL/al.h>
#include "AudioSource.h"

using namespace DrageEngine;

AudioSource::AudioSource()
{
    alGenSources(1, &id);
}

AudioSource::~AudioSource()
{
    Stop();
    alDeleteSources(1, &id);
}

void AudioSource::SetClip(const AudioClip *clip)
{
    alSourcei(id, AL_BUFFER, clip->GetID());
}

void AudioSource::Play(const AudioClip *clip)
{
    Stop();
    alSourcei(id, AL_BUFFER, clip->GetID());
    alSourcePlay(id);
}

void AudioSource::Play()
{
    alSourcePlay(id);
}

void AudioSource::Pause()
{
    alSourcePause(id);
}

void AudioSource::Stop()
{
    alSourceStop(id);
}

bool AudioSource::IsPlaying() const
{
    int state;
    alGetSourcei(id, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

float AudioSource::GetVolume() const
{
    return volume;
}

void AudioSource::SetVolume(float volume)
{
    volume = volume;
    alSourcef(id, AL_GAIN, volume);
}

float AudioSource::GetPitch() const
{
    return pitch;
}

void AudioSource::SetPitch(float pitch)
{
    pitch = pitch;
    alSourcef(id, AL_PITCH, pitch);
}

Vector3 AudioSource::GetPosition() const
{
    return position;
}

void AudioSource::SetPosition(const Vector3 &position)
{
    this->position = position;
    alSource3f(id, AL_POSITION, position.x, position.y, position.z);
}

Vector3 AudioSource::GetVelocity() const
{
    return velocity;
}

void AudioSource::SetVelocity(const Vector3 &velocity)
{
    this->velocity = velocity;
    alSource3f(id, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

bool AudioSource::IsLooping() const
{
    return looping;
}

void AudioSource::SetLooping(bool loop)
{
    looping = loop;
    alSourcei(id, AL_LOOPING, loop);
}
