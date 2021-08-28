
#include <stdlib.h>
#include <time.h>
#include "Random.h"

using namespace DrageEngine;

void Random::SetSeed()
{
    srand((unsigned)time(NULL));
}

void Random::SetSeed(unsigned seed)
{
    srand(seed);
}

float Random::Float(float min, float max)
{
    return (rand() / (float)RAND_MAX) * (max - min) + min;
}

int Random::Int(int min, int max)
{
    return rand() % (max - min) + min;
}

bool Random::Bool()
{
    return rand() % 2 == 0;
}

Quaternion Random::Rotation()
{
    return Quaternion::FromEuler(Float(0, 360), Float(0, 360), Float(0, 360));
}
