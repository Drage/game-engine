
#version 410 core

#include "Core.frag"

uniform sampler2D diffuseMap;
uniform sampler2D alphaMap;

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec2 fragTexCoord2;
in vec3 fragNormal;

void main()
{
    float alpha = texture(alphaMap, fragTexCoord2).r;
    vec4 linearColor = texture(diffuseMap, fragTexCoord);
    vec3 gamma = vec3(1.0 / 2.2);
    SetFinalOutput(vec4(pow(linearColor.xyz, gamma), alpha * linearColor.a));
}
