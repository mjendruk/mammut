#version 410

uniform sampler2D horizontalBlur;
uniform sampler2D normal_depth;
uniform sampler2D color;
uniform float farPlane;

layout (location = 0) out vec3 fragColor;

const int blurSize = 7;
const int offset = int(- blurSize * 0.5 + 0.5);

ivec2 v_uv = ivec2(gl_FragCoord.xy);

void main()
{
    vec4 fragNormalDepth = texelFetch(normal_depth, v_uv, 0);
    float blurred = 0;
    int sampleCount = 0;

    for (int i = 0; i < blurSize; ++i)
    {   
        ivec2 sampleUv = v_uv + ivec2(0, (i + offset));
        vec3 sampleNormal = texelFetch(normal_depth, sampleUv, 0).xyz;

        int considerSample = int(fragNormalDepth.xyz == sampleNormal);
        blurred += considerSample * texelFetch(horizontalBlur, sampleUv, 0).x;
        sampleCount += considerSample;
    }

    float ssaoFactor = blurred / sampleCount;
    float fogFactor = smoothstep(0.0, 1.0, 1 - sqrt(- fragNormalDepth.w / farPlane));
    fragColor = texelFetch(color, v_uv, 0).rgb * fogFactor * ssaoFactor;
}
