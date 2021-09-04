
#version 410 core

uniform vec4 colorOverride;

layout(location = 0) out vec4 outputFragColor;

void SetFinalOutput(vec4 fragColor)
{
    if (colorOverride != vec4(0))
    {
        outputFragColor = colorOverride;
        return;
    }
    outputFragColor = fragColor;
}
