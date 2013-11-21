#version 410

out vec4 fragColor;
in vec4 position;

void main()
{
    fragColor = vec4(vec3(position.x*0.05, position.y*0.1, position.z*0.1), 1.0);
}
