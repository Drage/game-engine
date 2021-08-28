
#include "Light.h"

using namespace DrageEngine;

Light::Light()
{
    m_type = DIRECTIONAL;
    m_attenuation = 1.0f;
    m_ambientCoefficient = 0.0f;
    m_coneAngle = 0.0f;
}

void Light::ApplyUniforms(const Shader *shader, int index) const
{
    shader->SetUniform("lights", index, "position", m_position);
    shader->SetUniform("lights", index, "intensities", m_color);
    shader->SetUniform("lights", index, "attenuation", m_attenuation);
    shader->SetUniform("lights", index, "ambientCoefficient", m_ambientCoefficient);
    shader->SetUniform("lights", index, "coneDirection", m_coneDirection);
    shader->SetUniform("lights", index, "coneAngle", m_coneAngle);
}
