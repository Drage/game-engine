
#include <OpenAL/al.h>
#include "AudioClip.h"

using namespace DrageEngine;

AudioClip::AudioClip()
{
    id = 0;
}

AudioClip::AudioClip(Audio *audio)
{
    id = 0;
    Generate(audio);
}

AudioClip::~AudioClip()
{
    alDeleteBuffers(1, &id);
}

unsigned AudioClip::Generate(Audio *audio)
{
    alGenBuffers(1, &id);
    alBufferData(id, audio->GetFormat(), audio->GetData(), audio->GetSize(), audio->GetFrequency());
    return id;
}

unsigned AudioClip::GetID() const
{
    return id;
}
