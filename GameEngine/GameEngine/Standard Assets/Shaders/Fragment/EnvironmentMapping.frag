
#version 410 core

uniform samplerCube cubemap;

vec4 ApplyReflection(vec3 cameraPosition, vec3 position, vec3 normal)
{
    vec3 incident = normalize(position - cameraPosition);
    vec3 reflected = reflect(incident, normalize(normal));
    return vec4(texture(cubemap, vec3(reflected.x, -reflected.y, reflected.z)).rgb, 1.0);
}

vec4 ApplyRefraction(vec3 cameraPosition, vec3 position, vec3 normal)
{
    float ratio = 1.00 / 1.52;
    vec3 incident = normalize(position - cameraPosition);
    vec3 reflected = refract(incident, normalize(normal), ratio);
    return vec4(texture(cubemap, vec3(reflected.x, -reflected.y, reflected.z)).rgb, 1.0);
}
