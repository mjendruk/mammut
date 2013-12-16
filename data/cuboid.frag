#version 410

layout (location = 0) out vec4 normalOut;
layout (location = 1) out vec4 colorOut;

flat in vec3 v_normal;

flat in vec4 v_color;
smooth in float v_eyeDistance;
smooth in float depthInES;

void main()
{
	vec3 normal = normalize(v_normal);
	normalOut = vec4(normal, depthInES);
    colorOut = clamp(v_color + v_eyeDistance * 0.005, 0.0, 1.0);

}
