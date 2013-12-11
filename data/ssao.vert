#version 410

uniform vec3 cameraPosition;
uniform mat4 projection;
uniform mat4 viewProjectionInv;
uniform mat4 view;

layout (location = 0) in vec2 a_vertex;
smooth out vec2 v_uv;
smooth out vec3 v_eyevector;

void main()
{
    // a_vertex in NDCS, (-1,-1)(1,1)
    vec4 vertexVC = inverse(projection) * vec4(a_vertex, 1.0, 1.0);
    v_eyevector = vertexVC.xyz / vertexVC.w * vec3(1.0, 1.0, -1.0);
	// todo: use real uv-coordinates
	v_uv = a_vertex * 0.5 + 0.5;

	gl_Position = vec4(a_vertex, 0.0, 1.0);
}
