#version 410

layout (location = 1) out vec3 colorOut;

in vec2 g_relativePosition;


void main()
{
    if (length(g_relativePosition) > 1)
        discard;
        
    colorOut = vec3(1.0);
}
