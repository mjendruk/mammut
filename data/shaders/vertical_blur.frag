#version 410

uniform sampler2D horizontalBlur;
uniform sampler2D normal_depth;
uniform sampler2D color;
uniform float farPlane;

layout (location = 0) out vec3 fragColor;

const int blurSize = 7;

in vec2 v_uv;
in vec2 v_sampleUvs[blurSize];


void main()
{
    vec4 fragNormalDepth = texture(normal_depth, v_uv);
    float blurred = 0;
    int sampleCount = 0;

    for (int i = 0; i < blurSize; ++i)
    {   
        vec3 sampleNormal = texture(normal_depth, v_sampleUvs[i]).xyz;

        int considerSample = int(fragNormalDepth.xyz == sampleNormal);
        blurred += considerSample * texture(horizontalBlur, v_sampleUvs[i]).x;
        sampleCount += considerSample;
    }

    float ssaoFactor = blurred / sampleCount;
    float fogFactor = smoothstep(0.0, 1.0, 1 - sqrt(- fragNormalDepth.w / farPlane));
    fragColor = texture(color, v_uv).rgb * fogFactor * ssaoFactor;
}
