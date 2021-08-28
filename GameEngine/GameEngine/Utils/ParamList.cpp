
#include <fstream>
#include "ParamList.h"
#include "Debug.h"

using namespace DrageEngine;

std::string ParamList::Value(const std::string &name) const
{
    ParamMapType::const_iterator i = m_params.find(name);
    if (i == m_params.end())
        return std::string();
    else
        return i->second;
}

void ParamList::Merge(const ParamList &other, bool otherOverride)
{
    for (ParamMapType::const_iterator i = other.m_params.begin(); i != other.m_params.end(); i++)
    {
        if (!otherOverride)
        {
            if (m_params[i->first] != "")
                continue;
        }
        m_params[i->first] = i->second;
    }
}

bool ParamList::Remove(const std::string &name)
{
    for (ParamMapType::iterator i = m_params.begin(); i != m_params.end(); i++)
    {
        if (i->first == name)
        {
            m_params.erase(i);
            return true;
        }
    }
    return false;
}

const ParamMapType& ParamList::GetParamMap() const
{
    return m_params;
}

void ParamList::Clear()
{
    m_params.clear();
}

bool ParamList::Load(const std::string &filename, ParamList &params)
{
    // Open file
    std::ifstream file(filename);
    
    // Check file is open
    if (!file.is_open())
    {
        ERROR("Unable to open file: " + filename);
        return false;
    }
    
    char buffer[256] = {0};
    file.getline(buffer, sizeof(buffer));
    while (file.good())
    {
        if (buffer[0] != ';' && buffer[0] != '[')
        {
            std::string line(buffer);
            params.Set(line.substr(0, line.find('=')), line.substr(line.find('=') + 1));
        }
        file.getline(buffer, sizeof(buffer));
    }
    
    return true;
}
