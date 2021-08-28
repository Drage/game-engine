
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include "AudioManager.h"
#include "Debug.h"

using namespace DrageEngine;

AudioManager::AudioManager()
{
    m_device = alcOpenDevice(NULL);
    if (!m_device)
    {
        ERROR("No audio device found");
        return;
    }
    
    m_context = alcCreateContext(m_device, NULL);
    if (!alcMakeContextCurrent(m_context))
    {
        ERROR("Could not initialize audio context");
        return;
    }
}

AudioManager::~AudioManager()
{
    alcMakeContextCurrent(NULL);
    alcDestroyContext(m_context);
    alcCloseDevice(m_device);
}
