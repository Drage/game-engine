
#version 410 core

#include "Core.frag"

uniform samplerCube cubemap;

in vec3 fragPosition;

void main()
{
    SetFinalOutput(texture(cubemap, vec3(fragPosition.x, -fragPosition.y, fragPosition.z)));
}
