#version 410

uniform sampler2D ssao;
uniform sampler2D normal_depth;

layout (location = 0) out float fragColor;

const int blurSize = 7;
const int offset = int(- blurSize * 0.5 + 0.5);
const float considerSampleDistance = 0.4;

ivec2 v_uv = ivec2(gl_FragCoord.xy);

void main()
{
    float depth = texelFetch(normal_depth, v_uv, 0).w;
    float blurred = 0;
    int sampleCount = 0;
    
    for (int i = 0; i < blurSize; ++i)
    {
        ivec2 sampleUv = v_uv + ivec2((i + offset), 0);
        float sampleDepth = texelFetch(normal_depth, sampleUv, 0).w;

        int considerSample = int(abs(depth - sampleDepth) < considerSampleDistance);
        blurred += considerSample * texelFetch(ssao, sampleUv, 0).x;
        sampleCount += considerSample;
    }

    fragColor = blurred / sampleCount;
}



