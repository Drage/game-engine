
#version 410 core

uniform mat4 modelViewProjectionMatrix;

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec2 vertTexCoord;
layout (location = 2) in vec3 vertNormal;

out vec3 fragPosition;
out vec2 fragTexCoord;
out vec3 fragNormal;

void main()
{
    fragPosition = vertPosition;
    fragTexCoord = vertTexCoord;
    fragNormal = vertNormal;
    
    gl_Position = modelViewProjectionMatrix * vec4(vertPosition, 1);
}
