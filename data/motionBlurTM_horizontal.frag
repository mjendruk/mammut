#version 410

uniform sampler2D tmVertical;
uniform float radius;
uniform vec2 viewport;

layout (location = 0) out vec2 fragColor;

smooth in vec2 v_uv;

void main() {

    vec2 m = vec2(0.0);
    float largestMagnitude2 = 0.0;

    int tileCornerX = int(gl_FragCoord.x) * int(radius);
    int tileY = int(gl_FragCoord.y);

    int maxCoordX = int(viewport.x) - 1;

    for(int offset =0; offset < int(radius); ++offset)
    {
        ivec2 G = ivec2(clamp(tileCornerX + offset, 0, maxCoordX), tileY);

        vec2 v_G = texelFetch(tmVertical, G, 0).rg;

        float thisMagnitude2 = dot(v_G, v_G);

        if(thisMagnitude2 > largestMagnitude2)
        {
            m = v_G;
            largestMagnitude2 = thisMagnitude2;
        }
    }

    fragColor = m;
}
