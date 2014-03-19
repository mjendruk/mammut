#version 410

uniform mat4 projection;
uniform mat4 view;
uniform mat4 previousView;
uniform vec3 lookAt;

layout (points) in;
layout (line_strip, max_vertices = 2) out;

void main()
{
    vec4 position = gl_in[0].gl_Position;

    vec4 first_position_ss_homo = previousView * position;
    vec4 second_position_ss_homo = view * position;

    vec3 first_position_ss = first_position_ss_homo.xyz / first_position_ss_homo.w;
    vec3 second_position_ss = second_position_ss_homo.xyz / second_position_ss_homo.w;

    gl_Position = projection * first_position_ss_homo;
    EmitVertex();

    gl_Position = projection * second_position_ss_homo;
    EmitVertex();

    // vec3 widthAxis = normalize(cross(lookAt, second_position_ss - first_position_ss)) + 1.0;

    // gl_Position = projection * vec4(first_position_ss + widthAxis, 1.0);
    // EmitVertex();

    // gl_Position = projection * vec4(second_position_ss + widthAxis, 1.0);
    // EmitVertex();

    // gl_Position = projection * vec4(first_position_ss - widthAxis, 1.0);
    // EmitVertex();

    // gl_Position = projection * vec4(second_position_ss - widthAxis, 1.0);
    // EmitVertex();
}
