
#version 410 core

uniform sampler2D diffuseMap;
uniform vec4 color;

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;

out vec4 fragColor;

void main()
{
    vec4 linearColor = texture(diffuseMap, fragTexCoord) * color;
    vec3 gamma = vec3(1.0 / 2.2);
    fragColor = vec4(pow(linearColor.xyz, gamma), linearColor.a);
}
