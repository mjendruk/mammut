#version 410

layout (location = 0) out vec4 normalOut;
layout (location = 1) out vec4 colorOut;
layout (location = 2) out vec2 velocityOut;

uniform mat4 viewProjection;
uniform mat4 prevViewProjection;

flat in vec3 v_normal;
flat in vec4 v_color;
smooth in float v_eyeDistance;
smooth in float v_depthInES;

smooth in vec4 v_currentPositionInWS;
smooth in vec4 v_previousPositionInWS;

void main()
{
	normalOut = vec4(v_normal, v_depthInES);
    colorOut =  clamp(v_color - v_eyeDistance, 0.0, 1.0);

    vec4 posTemp = viewProjection * v_currentPositionInWS;
    vec4 prevPosTemp = prevViewProjection * v_previousPositionInWS;

    vec2 position = (posTemp.xy / posTemp.w); 
    vec2 prevPosition = (prevPosTemp.xy / prevPosTemp.w); 

    velocityOut = position - prevPosition;
}
