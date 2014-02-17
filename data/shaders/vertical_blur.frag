#version 410

uniform sampler2D horizontalBlur;
uniform sampler2D normal_depth;
uniform sampler2D color;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;

const int blurSize = 7;

void main()
{
    float texelSize = 1.0 / textureSize(horizontalBlur, 0).y;
    vec3 normal = texture(normal_depth, v_uv).xyz;
    float blurred = 0;
    int offset = int(- blurSize * 0.5 + 0.5);
    int sampleCount = 0;
    for (int i = 0; i < blurSize; ++i)
    {
        vec2 sampleUv = v_uv + vec2(0.0, (i + offset) * texelSize);
        
        vec3 sampleNormal = texture(normal_depth, sampleUv).xyz;

        int considerSample = int(normal == sampleNormal);
        blurred += considerSample * texture(horizontalBlur, sampleUv).x;
        sampleCount += considerSample;
    }


    float ssaoFactor = blurred / sampleCount;
    fragColor = vec4(texture(color, v_uv).rgb * ssaoFactor, 1.0);
}
