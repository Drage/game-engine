
#version 410 core

uniform sampler2D diffuseMap;
uniform float shininess;
uniform vec4 color;

#define MAX_LIGHTS 10
uniform int numLights;

uniform struct Light {
    vec4 position;
    vec3 intensities;
    float attenuation;
    float ambientCoefficient;
    float coneAngle;
    vec3 coneDirection;
} lights[MAX_LIGHTS];

vec3 ApplyLight(Light light, vec3 surfaceColor, vec3 normal, vec3 surfacePosition, vec3 surfaceToCamera)
{
    vec3 surfaceToLight;
    float attenuation = 1.0;
    
    if (light.position.w == 0.0)
    {
        // Directional light
        surfaceToLight = -normalize(light.position.xyz);
        attenuation = 1.0;
    }
    else
    {
        // Point light
        surfaceToLight = normalize(light.position.xyz - surfacePosition);
        float distanceToLight = length(light.position.xyz - surfacePosition);
        attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));
        
        // Cone restrictions (affects attenuation)
        if (light.coneAngle > 0.0)
        {
            float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(light.coneDirection))));
            if (lightToSurfaceAngle > light.coneAngle)
                attenuation = 0.0;
        }
    }
    
    // Ambient
    vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensities;
    
    // Diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities;
    
    // Specular
    float specularCoefficient = 0.0;
    if (diffuseCoefficient > 0.0)
    {
        float shiny = pow(100000000, 1 - shininess);
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), shiny);
    }
    vec3 specular = specularCoefficient * light.intensities;
    
    return ambient + attenuation * (diffuse + specular);
}

vec4 ApplyLighting(mat4 modelMatrix, vec3 cameraPosition, vec3 position, vec3 normal, vec2 texCoord)
{
    vec3 surfaceNormal = normalize(transpose(inverse(mat3(modelMatrix))) * normal);
    vec3 surfacePosition = vec3(modelMatrix * vec4(position, 1));
    vec4 surfaceColor = texture(diffuseMap, texCoord) * color;
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePosition);
    
    // Combine color from all the lights
    vec3 linearColor = vec3(0);
    for (int i = 0; i < numLights; i++)
        linearColor += ApplyLight(lights[i], surfaceColor.rgb, surfaceNormal, surfacePosition, surfaceToCamera);
    
    // Final color (after gamma correction)
    vec3 gamma = vec3(1.0 / 2.2);
    return vec4(pow(linearColor, gamma), surfaceColor.a);
}
