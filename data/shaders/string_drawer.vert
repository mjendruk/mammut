#version 410

layout (location = 0) in vec2 a_vertex;
layout (location = 1) in mat4 a_vertexTransform;
layout (location = 5) in mat4 a_textureCoordTransform;

out vec2 v_textureCoord;

void main()
{
    vec4 textureCoord = a_textureCoordTransform * vec4(a_vertex, 0.0, 1.0);
    vec4 position = a_vertexTransform * vec4(a_vertex, 0.0, 1.0);

    v_textureCoord = textureCoord.xy / textureCoord.w;
    gl_Position = position;
}
