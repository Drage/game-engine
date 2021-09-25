
#version 410 core

uniform mat4 viewProjectionMatrix;

layout (location = 0) in vec3 vertPosition;

out vec3 fragPosition;

void main()
{
    fragPosition = vertPosition;
    
    mat4 viewProj = viewProjectionMatrix;
    viewProj[3] = vec4(0);
    
    gl_Position = (viewProj * vec4(vertPosition, 1.0)).xyww;
}
