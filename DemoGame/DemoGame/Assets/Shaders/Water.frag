
#version 410 core

uniform sampler2D diffuseMap;
uniform sampler2D alphaMap;

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec2 fragTexCoord2;
in vec3 fragNormal;

out vec4 fragColor;

void main()
{
    float alpha = texture(alphaMap, fragTexCoord2).r;
    vec4 linearColor = texture(diffuseMap, fragTexCoord);
    vec3 gamma = vec3(1.0 / 2.2);
    fragColor = vec4(pow(linearColor.xyz, gamma), alpha * linearColor.a);
}
