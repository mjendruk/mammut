#version 410

uniform sampler2D ssao;
uniform sampler2D normal_depth;

layout (location = 0) out float fragColor;

in vec2 v_uv;

const int blurSize = 7;

void main()
{
    float texelSize = 1.0 / textureSize(ssao, 0).x;
    vec3 normal = texture(normal_depth, v_uv).xyz;
    float blurred = 0;
    int offset = int(- blurSize * 0.5 + 0.5);
    int sampleCount = 0;
    for (int i = 0; i < blurSize; ++i)
    {
        vec2 sampleUv = v_uv + vec2((i + offset) * texelSize, 0.0);
        
        vec3 sampleNormal = texture(normal_depth, sampleUv).xyz;

        int considerSample = int(normal == sampleNormal);
        blurred += considerSample * texture(ssao, sampleUv).x;
        sampleCount += considerSample;
    }


    fragColor = blurred / sampleCount;
}



