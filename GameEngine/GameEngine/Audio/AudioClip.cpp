
#include <OpenAL/al.h>
#include "AudioClip.h"

using namespace DrageEngine;

AudioClip::AudioClip()
{
    m_id = 0;
}

AudioClip::AudioClip(Audio *audio)
{
    m_id = 0;
    Generate(audio);
}

AudioClip::~AudioClip()
{
    alDeleteBuffers(1, &m_id);
}

unsigned AudioClip::Generate(Audio *audio)
{
    alGenBuffers(1, &m_id);
    alBufferData(m_id, audio->GetFormat(), audio->GetData(), audio->GetSize(), audio->GetFrequency());
    return m_id;
}

unsigned AudioClip::GetID() const
{
    return m_id;
}
