#version 410

uniform mat4 projection;
uniform mat4 view;
uniform mat4 previousView;
uniform vec3 eye;

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 g_relativePosition; 


void main()
{
    vec4 position = gl_in[0].gl_Position;

    vec4 first_position_ss_homo = previousView * position;
    vec4 second_position_ss_homo = view * position;

    vec3 first_position_ss = first_position_ss_homo.xyz / first_position_ss_homo.w;
    vec3 second_position_ss = second_position_ss_homo.xyz / second_position_ss_homo.w;

    vec3 moveAxis = normalize(second_position_ss - first_position_ss) * 0.5;
    vec3 upAxis = normalize(cross(first_position_ss, moveAxis)) * 0.007;

    gl_Position = projection * vec4(second_position_ss + upAxis, 1.0);
    g_relativePosition = vec2(1.0, 1.0);
    EmitVertex();

    gl_Position = projection * vec4(second_position_ss - moveAxis + upAxis, 1.0);
    g_relativePosition = vec2(-1.0, 1.0);
    EmitVertex();

    gl_Position = projection * vec4(second_position_ss - upAxis, 1.0);
    g_relativePosition = vec2(1.0, -1.0);
    EmitVertex();

    gl_Position = projection * vec4(second_position_ss - moveAxis - upAxis, 1.0);
    g_relativePosition = vec2(-1.0, -1.0);
    EmitVertex();

    // gl_Position = projection * vec4(first_position_ss - upAxis, 1.0);

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
