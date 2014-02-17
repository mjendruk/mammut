#version 410

uniform mat4 inverseProjection;
uniform vec2 noiseScale;

smooth out vec2 v_uv;
smooth out vec2 v_noiseUv;
smooth out vec3 v_viewRay;

layout (location = 0) in vec2 a_vertex;


void main()
{
    vec4 vertex_vs = inverseProjection * vec4(a_vertex, 1.0, 1.0);
    v_viewRay = vertex_vs.xyz / vertex_vs.w;    
    
    v_uv = a_vertex * 0.5 + 0.5;
    v_noiseUv = v_uv * noiseScale;

    gl_Position = vec4(a_vertex, 0.0, 1.0);
}
