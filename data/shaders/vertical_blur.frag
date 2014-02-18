#version 410

uniform sampler2D horizontalBlur;
uniform sampler2D normal_depth;
uniform sampler2D color;

layout (location = 0) out vec3 fragColor;

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
        blurred += considerSample * texture(horizontalBlur, v_sampleUvs[i]).x;
        sampleCount += considerSample;
    }


    float ssaoFactor = blurred / sampleCount;
    fragColor = vec3(texture(color, v_uv).rgb * ssaoFactor);
}
