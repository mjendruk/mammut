#version 410

uniform sampler2D ssao;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;

const int blurSize = 8;

void main()
{
    float texelSize = 1.0 / textureSize(ssao, 0).x;

    float blurred = 0;
    int offset = int(- blurSize * 0.5 + 0.5);
    for (int i = 0; i < blurSize; ++i)
    {
        blurred += texture(ssao, v_uv + vec2((i + offset) * texelSize), 0.0).r;
    }


    fragColor = vec4(blurred / blurSize, vec3(0.0));
}



