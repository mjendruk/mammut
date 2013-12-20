#version 410

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 model;

in vec4 v_color[3];
in float v_depthInES[3];
in float v_eyeDistance[3];
in vec3 v_worldVertex[3];


smooth out float g_depthInES;
smooth out float g_eyeDistance;
flat out vec4 g_color;
flat out vec3 g_normal;


vec3 normalToWorldSpace(vec3 normal)
{
    vec3 transformed = vec3(model * vec4(normal, 0.0));
    return normalize(transformed);
}


void main() {


    vec3 a = v_worldVertex[0] - v_worldVertex[1];
    vec3 b = v_worldVertex[2] - v_worldVertex[1];

    vec3 normal = normalize(cross(a, b));

    for(int i = 0; i < 3; i++) {
    	g_depthInES = v_depthInES[i];
    	g_eyeDistance = v_eyeDistance[i];
        g_color = v_color[i];
        g_normal = normal;
        gl_Position = gl_in[i].gl_Position;

        EmitVertex();
    }

    EndPrimitive();
}