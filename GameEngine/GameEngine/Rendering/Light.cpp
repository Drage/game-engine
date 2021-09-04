
#include "Light.h"

using namespace DrageEngine;

Light::Light()
{
    type = DIRECTIONAL;
    attenuation = 1.0f;
    ambientCoefficient = 0.0f;
    coneAngle = 0.0f;
}
