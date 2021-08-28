
#version 410 core

#include "Lighting.frag"

uniform mat4 modelMatrix;
uniform vec3 cameraPosition;

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;

out vec4 fragColor;

void main()
{
    fragColor = ApplyLighting(modelMatrix, cameraPosition, fragPosition, fragNormal, fragTexCoord);
}
