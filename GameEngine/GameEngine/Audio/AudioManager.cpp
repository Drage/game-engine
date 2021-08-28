
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include "AudioManager.h"
#include "Debug.h"

using namespace DrageEngine;

AudioManager::AudioManager()
{
    device = alcOpenDevice(NULL);
    if (!device)
    {
        ERROR("No audio device found");
        return;
    }
    
    context = alcCreateContext(device, NULL);
    if (!alcMakeContextCurrent(context))
    {
        ERROR("Could not initialize audio context");
        return;
    }
}

AudioManager::~AudioManager()
{
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}
