#version 410

uniform mat4 transform;
uniform mat4 model;

in vec3 a_vertex;
out vec4 position;

void main()
{
    position = transform * model * vec4(a_vertex, 1.0);
    gl_Position = position;
}
