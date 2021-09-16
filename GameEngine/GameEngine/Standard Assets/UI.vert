
#version 410 core

uniform mat4 modelMatrix;
uniform int viewportWidth;
uniform int viewportHeight;
uniform int textureWidth;
uniform int textureHeight;

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec2 vertTexCoord;
layout (location = 2) in vec3 vertNormal;

out vec2 fragTexCoord;

void main()
{
    fragTexCoord = vertTexCoord;
    
    float xScale = float(textureWidth) / float(viewportWidth);
    float yScale = float(textureHeight) / float(viewportHeight);
    mat4 textureScale = mat4(mat2(xScale, 0, 0, yScale));
    
    gl_Position = modelMatrix * textureScale * vec4(vertPosition, 1);
}
