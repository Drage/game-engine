
#version 410 core

#include "Core.frag"

uniform sampler2D diffuseMap;
uniform vec4 color;

in vec2 fragTexCoord;

void main()
{
    vec4 linearColor = texture(diffuseMap, fragTexCoord) * color;
    vec3 gamma = vec3(1.0 / 2.2);
    SetFinalOutput(vec4(pow(linearColor.xyz, gamma), linearColor.a));
}
