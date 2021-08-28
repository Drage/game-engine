
#include <OpenAL/al.h>
#include <OpenAL/alut.h>
#include "AudioWAV.h"
#include "Debug.h"

using namespace DrageEngine;

bool AudioWAV::Load(const std::string &filename)
{
    alutLoadWAVFile((char*)filename.c_str(), &format, (void**)&data, &size, &frequency);
    return true;
}
