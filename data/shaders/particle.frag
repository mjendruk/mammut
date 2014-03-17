#version 410

layout (location = 0) out vec4 normalOut;
layout (location = 1) out vec3 colorOut;
layout (location = 2) out vec2 velocityOut;

flat in vec3 g_normal;
in float g_depth_vs;

void main()
{
    normalOut = vec4(g_normal, g_depth_vs);
    colorOut = vec3(1.0);
    velocityOut = vec2(0.0);
}
