
#include <math.h>
#include <limits>
#include "MathUtils.h"

const float DrageEngine::PI = M_PI;

const float DrageEngine::EPSILON = std::numeric_limits<float>::epsilon();

float DrageEngine::DegToRad(float degrees)
{
    return degrees / 180.0f * M_PI;
}

float DrageEngine::RadToDeg(float radians)
{
    return radians * 180.0f / M_PI;
}

int DrageEngine::PingPong(int value, int min, int max)
{
    int numWraps, excess;
    
    if (max - min == 0 || value == 0)
    {
        numWraps = 0;
        excess = value;
    }
    else
    {
        numWraps = value / (max - min);
        excess = (numWraps == 0) ? value : value % (max - min);
    }
    
    if (numWraps % 2 == 0)
        return min + excess;
    else
        return max - excess;
}

float DrageEngine::Normalize(float value, float min, float max)
{
    return (value - min) / (max - min);
}
