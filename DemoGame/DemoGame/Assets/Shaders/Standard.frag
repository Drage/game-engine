
#version 410 core

#include "Core.frag"
#include "Lighting.frag"

uniform mat4 modelMatrix;
uniform vec3 cameraPosition;

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;

void main()
{
    SetFinalOutput(ApplyLighting(modelMatrix, cameraPosition, fragPosition, fragNormal, fragTexCoord));
}
