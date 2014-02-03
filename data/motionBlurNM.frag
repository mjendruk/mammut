#version 410

uniform sampler2D maxTile;
uniform float radius;
uniform vec2 viewport;

layout (location = 0) out vec2 fragColor;

smooth in vec2 v_uv;

void main() {

    vec2 m = vec2(0.0);

    float largestMagnitude2 = -1.0;

    //das über viewport berechnen
    ivec2 maxCoord = textureSize(maxTile, 0) - ivec2(1);

    ivec2 currentTile = ivec2(gl_FragCoord.xy);
    ivec2 offset;

    for(offset.y = -1; offset.y <= 1; ++offset.y)
    {
        for(offset.x = -1; offset.x <= 1; ++offset.x)
        {
            ivec2 neighborTile = currentTile + offset;
            vec2 vmax_neighbor = texelFetch(maxTile, clamp(neighborTile, ivec2(0), maxCoord),0).rg;
        
            float magnitude2_neighbor = dot(vmax_neighbor, vmax_neighbor);

            if(magnitude2_neighbor > largestMagnitude2)
            {
                vec2 directionOfVelocity = vmax_neighbor;

                float displacement = abs(offset.x) + abs(offset.y);

                vec2 point = sign(offset * directionOfVelocity);

                float distance = (point.x + point.y);

                if(abs(distance) == displacement)
                {
                    m = vmax_neighbor;
                    largestMagnitude2 = magnitude2_neighbor;
                }
            }
        }
    }

    fragColor = m;
/*
    vec2 maxVelocity = texture(maxTile, v_uv * radius).rg;

    float maxStep = 0.0;

    for(int u=-1; u < 1; ++u)
    {
        for(int v=-1; v < 1; ++v)
        {
            vec2 textureOutput = texture(maxTile, v_uv  * radius + vec2(u,v) * length(1 / viewport)).rg;

            //if sparen durch step
            //wenn länge textureOutput länger als länge maxVelocity → stepValue = 1.0 → sonst 0.0
            float stepValue = step(length(textureOutput), length(maxVelocity)); 
            maxVelocity = stepValue * textureOutput + (1 - stepValue) * maxVelocity;

        }
    }

    fragColor = maxVelocity;*/
}
