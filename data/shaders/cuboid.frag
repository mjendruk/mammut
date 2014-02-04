#version 410

layout (location = 0) out vec4 normalOut;
layout (location = 1) out vec4 colorOut;

flat in vec3 v_normal;
flat in vec4 v_color;
smooth in float v_eyeDistance;
smooth in float v_depthInES;

void main()
{
	normalOut = vec4(v_normal, v_depthInES);

    colorOut =  clamp(v_color - v_eyeDistance, 0.0, 1.0);
}