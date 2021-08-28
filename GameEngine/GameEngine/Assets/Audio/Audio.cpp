
#include "Audio.h"
#include "FileSystem.h"
#include "Debug.h"

using namespace DrageEngine;

Audio::Audio()
{
    size = 0;
    frequency = 0;
    format = 0;
    data = NULL;
}

Audio::~Audio()
{
    if (data)
    {
        delete[] data;
        data = NULL;
    }
}

bool Audio::Load(const std::string &filename)
{
    name = GetFileName(filename);
    ERROR("Cannot load audio as generic base type: " + filename);
    return false;
}

int Audio::GetFormat() const
{
    return format;
}

int Audio::GetSize() const
{
    return size;
}

int Audio::GetFrequency() const
{
    return frequency;
}

unsigned char* Audio::GetData() const
{
    return data;
}

const std::string& Audio::GetName() const
{
    return name;
}
