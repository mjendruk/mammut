#version 410

uniform sampler2D result;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;
in vec3 v_eye;

void main()
{
	fragColor = texture(result, v_uv);

}
