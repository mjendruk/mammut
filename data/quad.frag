#version 410

uniform sampler2D color;
uniform sampler2D colorOutput;

//uniform sampler2D motionBlurOutput;


layout (location = 0) out vec4 fragColor;

in vec2 v_uv;
in vec3 v_eye;

void main()
{
	fragColor = texture(colorOutput, v_uv);
    //fragColor = texture(motionBlurOutput, v_uv);

}
