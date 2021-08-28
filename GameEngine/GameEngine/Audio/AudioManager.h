
#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <OpenAL/alc.h>

namespace DrageEngine
{
    class AudioManager
    {
        public:
            AudioManager();
            ~AudioManager();
        
        private:
            ALCdevice *device;
            ALCcontext *context;
        
    };
}

#endif
