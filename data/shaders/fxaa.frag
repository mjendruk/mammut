#version 410

#define FXAA_PC 1
#define FXAA_GLSL_130 1
#define FXAA_QUALITY__PRESET 13
//#define FXAA_GREEN_AS_LUMA 1
#include </Fxaa3_11.h>

uniform sampler2D buf0;
smooth in vec2 v_uv;

layout (location = 0) out vec3 fragColor;


void main( void ) {
        //fragColor = texture(buf0,v_uv).xyz;
        //return;

        vec2 rcpFrame = 1.0 / textureSize(buf0, 0);
        float fxaaQualitySubpix = 0.75;
        float fxaaQualityEdgeThreshold = 0.166;
        float fxaaQualityEdgeThresholdMin = 0.0625;
        fragColor = FxaaPixelShader(v_uv, vec4(0.0), buf0, buf0, buf0, rcpFrame, vec4(0.0), vec4(0.0), vec4(0.0), fxaaQualitySubpix, fxaaQualityEdgeThreshold, fxaaQualityEdgeThresholdMin, 0.0, 0.0, 0.0, vec4(0.0)).xyz;
}
