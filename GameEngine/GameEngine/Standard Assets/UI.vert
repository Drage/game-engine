
#version 410 core

uniform mat4 modelMatrix;
uniform mat4 orthoProjectionMatrix;
uniform vec2 origin;
uniform bool nativeTextureScale;
uniform int textureWidth;
uniform int textureHeight;

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec2 vertTexCoord;
layout (location = 2) in vec3 vertNormal;

out vec2 fragTexCoord;

void main()
{
    fragTexCoord = vertTexCoord;
    
    float modelScaleX = modelMatrix[0][0];
    float modelScaleY = modelMatrix[1][1];
    vec4 originOffset = vec4(-modelScaleX / 2 * origin.x, -modelScaleY / 2 * origin.y, 0, 0);
    
    mat4 textureScale = mat4(1.0);
    if (nativeTextureScale)
        textureScale = mat4(mat2(textureWidth, 0, 0, textureHeight));
    
    gl_Position = orthoProjectionMatrix * (modelMatrix * textureScale * vec4(vertPosition, 1) + textureScale * originOffset);
}
