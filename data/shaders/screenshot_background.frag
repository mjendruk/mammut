#version 410

uniform sampler2D source;
in vec2 v_uv;

layout (location = 0) out vec4 fragColor;

const int blurSize = 31;

void main() 
{
    vec2 texelSize = 1.0 / vec2(textureSize(source, 0));
    
    vec4 color = texture(source, v_uv);

    fragColor = vec4(vec3((0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b) - 0.1), color.a);
}
