#version 410

layout (location = 0) out vec4 normalOut;
layout (location = 1) out vec4 colorOut;
//layout (location = 2) out vec2 velocityOut;

uniform vec2 viewport;
uniform float radius;

flat in vec3 v_normal;
flat in vec4 v_color;
smooth in float v_eyeDistance;
smooth in float v_depthInES;

smooth in vec4 v_currentPositionInSS;
smooth in vec4 v_previousPositionInSS;

void main()
{
	normalOut = vec4(v_normal, v_depthInES);

    colorOut =  clamp(v_color - v_eyeDistance, 0.0, 1.0);

    vec2 position = (v_currentPositionInSS.xy / v_currentPositionInSS.w) * 0.5 + 0.5; 
    vec2 prevPosition = (v_previousPositionInSS.xy / v_previousPositionInSS.w) * 0.5 + 0.5; 

    vec2 velocity = abs(position - prevPosition);
    if(length(velocity) > radius * length(1/viewport))
    {
        velocity /= length(velocity);
        velocity *= radius * length(1/viewport);
    }
   // velocityOut = velocity;// * max(0.5 * length(1/viewport), min( length(velocity), length(radius / viewport) ));
}
