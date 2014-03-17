#version 410

uniform mat4 projection;
uniform mat4 view;
uniform vec3 eye;

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

flat out vec3 g_normal;
smooth out float g_depth_vs;

void main()
{
    float scale = 0.1;
    vec3 position = gl_in[0].gl_Position.xyz;

    vec3 zAxis = vec3(0.0, 0.0, -1.0) * 10.0;
    vec3 xyAxis = normalize(cross(eye - position, zAxis)) * 0.1;

    g_normal = normalize(eye - position);

    vec4 position_vs = vec4(0.0);

    position_vs = view * vec4(position + (xyAxis + zAxis) * scale, 1.0);
    g_depth_vs = position_vs.z / position_vs.w;
    gl_Position = projection * position_vs;
    EmitVertex();

    position_vs = view * vec4(position + (- xyAxis + zAxis) * scale, 1.0);
    g_depth_vs = position_vs.z / position_vs.w;
    gl_Position = projection * position_vs;
    EmitVertex();

    position_vs = view * vec4(position + (xyAxis - zAxis) * scale, 1.0);
    g_depth_vs = position_vs.z / position_vs.w;
    gl_Position = projection * position_vs;
    EmitVertex();

    position_vs = view * vec4(position + (- xyAxis - zAxis) * scale, 1.0);
    g_depth_vs = position_vs.z / position_vs.w;
    gl_Position = projection * position_vs;
    EmitVertex();
}
