
#version 410 core

uniform mat4 modelViewProjectionMatrix;
uniform float time;

uniform float textureScale;
uniform float textureScrollSpeed;
uniform vec2 textureScrollDirection;

uniform vec3 center;
uniform float waveSpeed;
uniform float waveAmplitude;
uniform float waveLength;

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec2 vertTexCoord;
layout (location = 2) in vec3 vertNormal;
layout (location = 3) in vec2 vertTexCoord2;

out vec3 fragPosition;
out vec2 fragTexCoord;
out vec2 fragTexCoord2;
out vec3 fragNormal;

void main()
{
    float distToCenter = distance(center, vertPosition);
    float height = (sin(distToCenter / waveLength + waveSpeed * time) + 1) / 2 * waveAmplitude;
    fragPosition = vec3(vertPosition.x, height, vertPosition.z);
    
    fragNormal = vertNormal;
    
    vec2 textureOffset = textureScrollDirection.xy * mod(time * textureScrollSpeed, 1);
    fragTexCoord = vertTexCoord / textureScale + textureOffset;
    
    fragTexCoord2 = vertTexCoord2;
    
    gl_Position = modelViewProjectionMatrix * vec4(fragPosition, 1);
}
