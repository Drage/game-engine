
#include <algorithm>
#include "AudioFactory.h"
#include "AudioWAV.h"

using namespace DrageEngine;

AudioFactory::AudioFactory()
{
    // Register audio types with factory
    m_factory.Register<AudioWAV>("wav");
}

Audio* AudioFactory::Create(const std::string &extension)
{
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
    return m_factory.Create(ext);
}

bool AudioFactory::ExtensionSupported(const std::string &extension) const
{
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
    return m_factory.IsSupported(ext);
}
