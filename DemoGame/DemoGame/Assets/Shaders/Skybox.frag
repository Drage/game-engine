
#version 410 core

uniform samplerCube cubemap;

in vec3 fragPosition;

out vec4 fragColor;

void main()
{
    fragColor = texture(cubemap, vec3(fragPosition.x, -fragPosition.y, fragPosition.z));
}
