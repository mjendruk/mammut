#version 410

uniform sampler2D characterAtlas;

layout (location = 0) out vec4 fragColor;

in vec2 v_textureCoord;

void main() {
    float alpha = step(0.5, texture(characterAtlas, v_textureCoord).r);
    fragColor = vec4(0.0, 0.0, 0.0, alpha);
}
