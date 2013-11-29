#version 410

uniform sampler2D ssao;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;

void main()
{
	fragColor = vec4(texture2D(ssao, v_uv));
}