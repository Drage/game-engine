
#ifndef AUDIOLISTENERCOMPONENT_H
#define AUDIOLISTENERCOMPONENT_H

#include "Component.h"
#include "AudioListener.h"

namespace DrageEngine
{
    class AudioListenerComponent : public Component, public AudioListener
    {
        public:
            void Update();
        
        private:
            Vector3 lastPosition;
    };
}

#endif
