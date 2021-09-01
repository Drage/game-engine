
#version 410 core

uniform bool editorSelectMode;
uniform vec4 editorIndexColor;
uniform bool editorSelectionStencil;

smooth in vec2 coords;

layout(location = 0) out vec4 outputFragColor;

void SetFinalOutput(vec4 fragColor)
{
    if (editorSelectMode)
    {
        outputFragColor = editorIndexColor;
        return;
    }
    
    if (editorSelectionStencil)
    {
        outputFragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }
    
    outputFragColor = fragColor;
}
