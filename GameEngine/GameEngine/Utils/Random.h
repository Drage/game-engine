
#ifndef RANDOM_H
#define RANDOM_H

#include "Quaternion.h"

namespace DrageEngine
{
    class Random
    {
        public:
            static void SetSeed();
            static void SetSeed(unsigned seed);
            static float Float(float min, float max);
            static int Int(int min, int max);
            static bool Bool();
            static Quaternion Rotation();
    };
}

#endif
