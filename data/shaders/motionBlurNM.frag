#version 410

uniform sampler2D maxTile;
uniform float radius;
uniform vec2 viewport;

layout (location = 0) out vec2 fragColor;

smooth in vec2 v_uv;

void main() {

    vec2 m = vec2(0.0);

    float largestMagnitude2 = -1.0;

    ivec2 maxCoord = ivec2(viewport) - ivec2(1.0);

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
}
