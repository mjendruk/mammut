#version 410

uniform sampler2D normal;
uniform sampler2D depth;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;

void main()
{
	fragColor = vec4(texture2D(normal, v_uv));
}