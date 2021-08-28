
#ifndef AUDIOSOURCECOMPONENT_H
#define AUDIOSOURCECOMPONENT_H

#include <string>
#include "Component.h"
#include "AudioSource.h"
#include "AudioClip.h"

namespace DrageEngine
{
    class AudioSourceComponent : public Component, public AudioSource
    {
        public:
            void Init(const ParamList &params);
            void Update();
        
        private:
            AudioClip *clip;
            Vector3 lastPosition;
    };
}

#endif
