#version 410

uniform vec3 cameraPosition;
uniform mat4 projection;
uniform mat4 viewProjectionInv;

layout (location = 0) in vec2 a_vertex;
smooth out vec2 v_uv;
smooth out vec3 v_eyevector;

void main()
{
    vec4 temp = viewProjectionInv * vec4(a_vertex, 0.0, 1.0);
    v_eyevector = normalize(temp.xyz / temp.w - cameraPosition);
	v_uv = a_vertex * 0.5 + 0.5;
	gl_Position = vec4(a_vertex, 0.0, 1.0);
}
