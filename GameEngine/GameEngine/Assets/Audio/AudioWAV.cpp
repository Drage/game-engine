
#include <OpenAL/al.h>
#include <OpenAL/alut.h>
#include "AudioWAV.h"
#include "Debug.h"

using namespace DrageEngine;

bool AudioWAV::Load(const std::string &filename)
{
    alutLoadWAVFile((char*)filename.c_str(), &m_format, (void**)&m_data, &m_size, &m_frequency);
    return true;
}
