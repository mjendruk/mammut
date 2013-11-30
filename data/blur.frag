#version 410

uniform sampler2D ssao;

layout (location = 0) out vec4 fragColor;

const int blurSize = 4;
in vec2 v_uv;

void main2()
{
	fragColor = vec4(texture2D(ssao, v_uv));
}

void main() {
	vec2 texelSize = 1.0 / vec2(textureSize(ssao, 0));
	
//	ideally use a fixed size noise and blur so that this loop can be unrolled
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
	
	fragColor = fragColor / (blurSize * blurSize);
	//fragColor = vec4(1.0);
}