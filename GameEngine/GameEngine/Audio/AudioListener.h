
#ifndef AUDIOLISTENER_H
#define AUDIOLISTENER_H

#include "Vector3.h"
#include "Quaternion.h"

namespace DrageEngine
{
    class AudioListener
    {
        public:
            void SetPosition(const Vector3 &position);
            void SetVelocity(const Vector3 &velocity);
            void SetOrientation(const Quaternion &rotation);
    };
}

#endif
