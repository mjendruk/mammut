#version 410

uniform mat4 invProjection;
uniform vec2 viewport;
uniform int noiseTexSize;

smooth out vec2 v_uv;
smooth out vec2 v_noiseUv;
smooth out vec3 v_viewRay;

layout (location = 0) in vec2 a_vertex;


void main()
{
    vec4 vertex_vs = invProjection * vec4(a_vertex, 1.0, 1.0);
    v_viewRay = vertex_vs.xyz / vertex_vs.w;    
    
    v_uv = a_vertex * 0.5 + 0.5;
    v_noiseUv = viewport / vec2(noiseTexSize) * v_uv;

    gl_Position = vec4(a_vertex, 0.0, 1.0);
}
