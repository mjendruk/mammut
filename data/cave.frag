#version 410

layout (location = 0) out vec4 normalOut;
layout (location = 1) out vec4 colorOut;

flat in vec3 g_normal;

flat in vec4 g_color;
smooth in float g_eyeDistance;
smooth in float g_depthInES;

void main()
{
	vec3 normal = normalize(g_normal);
	normalOut = vec4(normal, g_depthInES);

    colorOut =  clamp(g_color + g_eyeDistance * 0.001, 0.0, 1.0);

}
