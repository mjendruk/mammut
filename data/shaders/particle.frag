#version 410

layout (location = 1) out vec4 colorOut;

in vec2 g_relativePosition;


void main()
{
    if (length(g_relativePosition) > 1)
        discard;
        
    colorOut = vec4(vec3(0.1), 0.8);
}
