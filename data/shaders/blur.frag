#version 410

uniform sampler2D color;
uniform sampler2D ssao;

layout (location = 0) out vec4 fragColor;

const int blurSize = 4;
in vec2 v_uv;

void main() {
	vec2 texelSize = 1.0 / vec2(textureSize(ssao, 0));
	
	fragColor = vec4(0.0);
	vec2 hlim = vec2(float(-blurSize) * 0.5 + 0.5);
	for (int x = 0; x < blurSize; ++x) {
		for (int y = 0; y < blurSize; ++y) {
			vec2 offset = vec2(float(x), float(y));
			offset += hlim;
			offset *= texelSize;
					
			fragColor += texture(ssao, v_uv + offset);
		}
	}
	
	vec4 ssaoFactor = fragColor / (blurSize * blurSize);
    fragColor = vec4(vec3(texture(color, v_uv) * ssaoFactor.r), 1.0);
}
