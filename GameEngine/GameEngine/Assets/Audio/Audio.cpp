
#include "Audio.h"
#include "FileSystem.h"
#include "Debug.h"

using namespace DrageEngine;

Audio::Audio()
{
    m_size = 0;
    m_frequency = 0;
    m_format = 0;
    m_data = NULL;
}

Audio::~Audio()
{
    if (m_data)
    {
        delete[] m_data;
        m_data = NULL;
    }
}

bool Audio::Load(const std::string &filename)
{
    m_name = GetFileName(filename);
    ERROR("Cannot load audio as generic base type: " + filename);
    return false;
}

int Audio::GetFormat() const
{
    return m_format;
}

int Audio::GetSize() const
{
    return m_size;
}

int Audio::GetFrequency() const
{
    return m_frequency;
}

unsigned char* Audio::GetData() const
{
    return m_data;
}

const std::string& Audio::GetName() const
{
    return m_name;
}
