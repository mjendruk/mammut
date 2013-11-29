#version 410

uniform sampler2D normal;
uniform sampler2D color;
uniform sampler2D depth;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;
in vec3 v_eye;

void main()
{
	fragColor = vec4(texture2D(color, v_uv));
}