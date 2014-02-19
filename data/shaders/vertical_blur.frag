#version 410

uniform sampler2D horizontalBlur;
uniform sampler2D normal_depth;
uniform sampler2D color;
uniform vec2 texelSize;

layout (location = 0) out vec3 fragColor;

const int blurSize = 7;
const int offset = int(- blurSize * 0.5 + 0.5);

in vec2 v_uv;

void main()
{
    vec3 normal = texture(normal_depth, v_uv).xyz;
    float blurred = 0;
    int sampleCount = 0;

    for (int i = 0; i < blurSize; ++i)
    {   
        vec2 sampleUv = v_uv + vec2(0.0, (i + offset) * texelSize.y);
        vec3 sampleNormal = texture(normal_depth, sampleUv).xyz;

        int considerSample = int(normal == sampleNormal);
        blurred += considerSample * texture(horizontalBlur, sampleUv).x;
        sampleCount += considerSample;
    }


    float ssaoFactor = blurred / sampleCount;
    fragColor = vec3(texture(color, v_uv).rgb * ssaoFactor);
}
