#version 410

uniform sampler2D ssao;
uniform sampler2D normal_depth;

layout (location = 0) out float fragColor;

const int blurSize = 7;
const int offset = int(- blurSize * 0.5 + 0.5);
const float considerSampleDistance = 0.8;
const float considerSampleNormalDifference = 0.92;

ivec2 v_uv = ivec2(gl_FragCoord.xy);

void main()
{
    vec4 normalDepth = texelFetch(normal_depth, v_uv, 0);
    float blurred = 0;
    int sampleCount = 0;
    
    for (int i = 0; i < blurSize; ++i)
    {
        ivec2 sampleUv = v_uv + ivec2((i + offset), 0);
        vec4 sampleNormalDepth = texelFetch(normal_depth, sampleUv, 0);

        bool depthOK = abs(normalDepth.w - sampleNormalDepth.w) < considerSampleDistance;
        bool normalOK = dot(normalDepth.xyz, sampleNormalDepth.xyz) > considerSampleNormalDifference;
        int considerSample = int(depthOK && normalOK);
        blurred += considerSample * texelFetch(ssao, sampleUv, 0).x;
        sampleCount += considerSample;
    }

    fragColor = blurred / sampleCount;
}



