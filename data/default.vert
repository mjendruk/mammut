#version 410

uniform mat4 transform;
uniform mat4 model;

in vec3 a_vertex;

void main()
{
    gl_Position = transform * model * vec4(a_vertex, 1.0);
}
