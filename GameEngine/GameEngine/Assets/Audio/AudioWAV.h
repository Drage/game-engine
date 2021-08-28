
#ifndef AUDIOWAV_H
#define AUDIOWAV_H

#include "Audio.h"

namespace DrageEngine
{
    class AudioWAV : public Audio
    {
        public:
            bool Load(const std::string &filename);
        
        private:
        
    };
}

#endif
