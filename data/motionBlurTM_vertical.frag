#version 410

uniform sampler2D velocity;
uniform float radius;
uniform vec2 viewport;

layout (location = 0) out vec2 fragColor;

smooth in vec2 v_uv;

void main() {

    vec2 m = vec2(0.0);
    float largestMagnitude2 = 0.0;

    int tileCornerY = int(gl_FragCoord.y) * int(radius);
    int tileX = int(gl_FragCoord.x);

    int maxCoordY = int(viewport.y) - 1;

    for(int offset = 0; offset < int(radius); ++offset)
    {
        ivec2 G = ivec2(tileX, clamp(tileCornerY + offset, 0, maxCoordY));

        vec2 v_G = texelFetch(velocity, G, 0).rg;

        float thisMagnitude2 = dot(v_G, v_G);

        if(thisMagnitude2 > largestMagnitude2)
        {
            m = v_G;
            largestMagnitude2 = thisMagnitude2;
        }
    }

    fragColor = m;
/*
    if((gl_FragCoord.x > 450.0) && (gl_FragCoord.y > 30.0))
    {
        fragColor = vec2(1.0,0.0);
        return;
    }

    if(gl_FragCoord.x > 450.0)
    {
        fragColor = vec2(0.0,1.0);
        return;
    }  

    if(gl_FragCoord.y > 30.0)
    {
        fragColor = vec2(1.0,1.0);
        return;
    }*/

/*

    ivec2 v_uvRadius = ivec2(gl_FragCoord.xy * radius);//v_uv * radius;
    vec2 maxVelocity = texelFetch(velocity, v_uvRadius, 0).rg;

    for(int u=0; u < radius; u++)
    {
        for(int v=0; v < radius; v++)
        {
            vec2 textureOutput = texelFetch(velocity, v_uvRadius + ivec2(u,v), 0).rg;

            //if sparen durch step
            //wenn länge textureOutput länger als länge maxVelocity → stepValue = 1.0 → sonst 0.0
            float stepValue = step(length(textureOutput), length(maxVelocity)); 
            maxVelocity = stepValue * textureOutput + (1 - stepValue) * maxVelocity;
        }
    }

    fragColor = maxVelocity;*/
}
