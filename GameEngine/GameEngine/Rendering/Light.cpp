
#include "Light.h"

using namespace DrageEngine;

Light::Light()
{
    type = DIRECTIONAL;
    attenuation = 1.0f;
    ambientCoefficient = 0.0f;
    coneAngle = 0.0f;
}

void Light::ApplyUniforms(const Shader *shader, int index) const
{
    shader->SetUniform("lights", index, "position", position);
    shader->SetUniform("lights", index, "intensities", color);
    shader->SetUniform("lights", index, "attenuation", attenuation);
    shader->SetUniform("lights", index, "ambientCoefficient", ambientCoefficient);
    shader->SetUniform("lights", index, "coneDirection", coneDirection);
    shader->SetUniform("lights", index, "coneAngle", coneAngle);
}
