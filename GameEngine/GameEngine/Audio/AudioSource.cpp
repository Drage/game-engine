
#include <OpenAL/al.h>
#include "AudioSource.h"

using namespace DrageEngine;

AudioSource::AudioSource()
{
    alGenSources(1, &m_id);
}

AudioSource::~AudioSource()
{
    Stop();
    alDeleteSources(1, &m_id);
}

void AudioSource::SetClip(const AudioClip *clip)
{
    alSourcei(m_id, AL_BUFFER, clip->GetID());
}

void AudioSource::Play(const AudioClip *clip)
{
    Stop();
    alSourcei(m_id, AL_BUFFER, clip->GetID());
    alSourcePlay(m_id);
}

void AudioSource::Play()
{
    alSourcePlay(m_id);
}

void AudioSource::Pause()
{
    alSourcePause(m_id);
}

void AudioSource::Stop()
{
    alSourceStop(m_id);
}

bool AudioSource::IsPlaying() const
{
    int state;
    alGetSourcei(m_id, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

float AudioSource::GetVolume() const
{
    return m_volume;
}

void AudioSource::SetVolume(float volume)
{
    m_volume = volume;
    alSourcef(m_id, AL_GAIN, m_volume);
}

float AudioSource::GetPitch() const
{
    return m_pitch;
}

void AudioSource::SetPitch(float pitch)
{
    m_pitch = pitch;
    alSourcef(m_id, AL_PITCH, pitch);
}

Vector3 AudioSource::GetPosition() const
{
    return m_position;
}

void AudioSource::SetPosition(const Vector3 &position)
{
    m_position = position;
    alSource3f(m_id, AL_POSITION, m_position.x, m_position.y, m_position.z);
}

Vector3 AudioSource::GetVelocity() const
{
    return m_velocity;
}

void AudioSource::SetVelocity(const Vector3 &velocity)
{
    m_velocity = velocity;
    alSource3f(m_id, AL_VELOCITY, m_velocity.x, m_velocity.y, m_velocity.z);
}

bool AudioSource::IsLooping() const
{
    return m_looping;
}

void AudioSource::SetLooping(bool loop)
{
    m_looping = loop;
    alSourcei(m_id, AL_LOOPING, loop);
}
