
#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H

#include "Audio.h"

namespace DrageEngine
{
    class AudioClip
    {
        public:
            AudioClip();
            AudioClip(Audio *audio);
            ~AudioClip();
            
            unsigned Generate(Audio *audio);
            unsigned GetID() const;
        
        private:
            unsigned id;
    };
}

#endif
