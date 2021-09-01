
#version 410 core

uniform sampler2D silhouette;
uniform int width;
uniform vec4 color;

in vec2 fragTexCoord;

layout(location = 0) out vec4 outputFragColor;

void main()
{
    if (texture(silhouette, fragTexCoord).xyz == vec3(0.0f))
    {
        vec2 size = 1.0f / textureSize(silhouette, 0);

        for (int i = -width; i <= width; i++)
        {
            for (int j = -width; j <= width; j++)
            {
                if (i == 0 && j == 0)
                    continue;

                vec2 offset = vec2(i, j) * size;

                if (texture(silhouette, fragTexCoord + offset).xyz == vec3(1.0f))
                {
                    outputFragColor = color;
                    return;
                }
            }
        }
    }

    discard;
}
