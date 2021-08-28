
#version 410 core

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

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
    
    mat4 m = modelViewMatrix;
    
    vec3 scale = vec3(length(m[0].xyz), length(m[1].xyz), length(m[2].xyz));

    m[0].xyz = vec3(scale.x, 0, 0);
    m[1].xyz = vec3(0, scale.y, 0);
    m[2].xyz = vec3(0, 0, scale.z);

    gl_Position = projectionMatrix * m * vec4(vertPosition, 1);
}
