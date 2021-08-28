
#ifndef AUDIOFACTORY_H
#define AUDIOFACTORY_H

#include <string>
#include "Factory.h"
#include "Audio.h"

namespace DrageEngine
{
    class AudioFactory
    {
        public:
            AudioFactory();
            Audio* Create(const std::string &extension);
            bool ExtensionSupported(const std::string &extension) const;
        
        private:
            Factory<Audio, std::string> factory;
    };
}

#endif
