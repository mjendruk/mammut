#version 410

uniform sampler2D ssao;
uniform vec2 texelSize;

const int blurSize = 7;
const int offset = int(- blurSize * 0.5 + 0.5);

out vec2 v_uv;
out vec2 v_sampleUvs[blurSize];

layout (location = 0) in vec2 a_vertex;

void main()
{
    v_uv = a_vertex * 0.5 + 0.5;

    for (int i = 0; i < blurSize; ++i)
    {
        v_sampleUvs[i] = v_uv + vec2(0.0, (i + offset) * texelSize.y);
    }

    gl_Position = vec4(a_vertex, 0.0, 1.0);
}
