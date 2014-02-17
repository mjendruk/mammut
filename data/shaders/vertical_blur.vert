#version 410

uniform sampler2D ssao;

const int blurSize = 7;
const int offset = int(- blurSize * 0.5 + 0.5);

out vec2 v_uv;
out vec2 v_sampleUvs[blurSize];

layout (location = 0) in vec2 a_vertex;

void main()
{
    v_uv = a_vertex * 0.5 + 0.5;

    float texelSize = 1.0 / textureSize(ssao, 0).y;

    for (int i = 0; i < blurSize; ++i)
    {
        v_sampleUvs[i] = v_uv + vec2(0.0, (i + offset) * texelSize);
    }

    gl_Position = vec4(a_vertex, 0.0, 1.0);
}
