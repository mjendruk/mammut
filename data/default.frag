#version 410

flat in vec4 v_color;
smooth in float v_eyeDistance;

out vec4 fragColor;

void main()
{
    fragColor = clamp(v_color + v_eyeDistance * 0.01, 0.0, 1.0);
}
