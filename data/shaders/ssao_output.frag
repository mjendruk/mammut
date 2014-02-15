#version 410

uniform sampler2D color;
uniform sampler2D ssao;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;

void main()
{
    float ssaoFactor = texture(ssao, v_uv).r;
    fragColor = vec4(vec3(texture(color, v_uv) * ssaoFactor), 1.0);
}

