#version 410

uniform sampler2D ssao;
uniform sampler2D normal_depth;

layout (location = 0) out float fragColor;

const int blurSize = 7;

in vec2 v_uv;
in vec2 v_sampleUvs[blurSize];

void main()
{
    vec3 normal = texture(normal_depth, v_uv).xyz;
    float blurred = 0;
    int sampleCount = 0;
    
    for (int i = 0; i < blurSize; ++i)
    {
        vec3 sampleNormal = texture(normal_depth, v_sampleUvs[i]).xyz;

        int considerSample = int(normal == sampleNormal);
        blurred += considerSample * texture(ssao, v_sampleUvs[i]).x;
        sampleCount += considerSample;
    }

    fragColor = blurred / sampleCount;
}



