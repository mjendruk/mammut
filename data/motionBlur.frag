#version 410

uniform sampler2D depth;
uniform sampler2D color;
uniform sampler2D velocity;
uniform sampler2D neighborMax;

uniform sampler2D randomBuffer;
//const int RANDOMBUFFER_SIZE = 32;
//uniform float randomBuffer[RANDOMBUFFER_SIZE];

uniform vec2 viewport;
uniform float radius;
uniform float numSamples;
uniform float currentFPS_targetFPS;

layout (location = 0) out vec4 fragColor;

smooth in vec2 v_uv;

float hash(ivec2 c) 
{
    if(numSamples <= 5)
        return float(int(c.x + c.y) & 1) * 0.5 + 0.25;
    else
        return texelFetch(randomBuffer, ivec2(c.x & 31, c.y & 31), 0).r;
}

vec2 readAdjustedVelocity(ivec2 C, sampler2D sampler, out float r)
{
    vec2 q = texelFetch(sampler, C, 0).xy;
    float lenq = length(q);

    r = (lenq * 0.5) * currentFPS_targetFPS;
    bool rMuchLargerThanZero = (r >= 0.01);

    //HALF_PIX = 0.5
    r = clamp(r, 0.5, radius);

    if(rMuchLargerThanZero)
        return q * (r / lenq);
    else
        return q;
}

vec2 readAdjustedVelocity(ivec2 C, out float r)
{
    return readAdjustedVelocity(C, velocity, r);
}

vec2 readAdjustedNeighborhoodVelocity(ivec2 C, out float r)
{
    return readAdjustedVelocity(ivec2(C / radius), neighborMax, r);
}

float cone(float dist, float r)
{
    return clamp(1.0 - abs(dist) / r, 0.0, 1.0);
}

float fastCone(float dist, float invR)
{
    return clamp(1.0 - abs(dist) * invR, 0.0, 1.0);
}

float cylinder(float dist, float r)
{
    return sign(r - abs(dist)) * 0.5 + 0.5;
}

float softDepthCompare(float depth_A, float depth_B)
{
    const float SOFT_DEPTH_EXTENT = 0.02;

    return clamp(1.0 - (depth_B - depth_A) / SOFT_DEPTH_EXTENT, 0.0, 1.0);
}

float softZCompare(float z_A, float z_B)
{
    const float SOFT_Z_EXTENT = 0.1;

    return clamp(1.0 - (z_A - z_B) / SOFT_Z_EXTENT, 0.0, 1.0);
}

void main()
{
    ivec2 screensize = ivec2(viewport);

    ivec2 me = ivec2(gl_FragCoord.xy);

    float jitter = hash(me) - 0.5;

    vec3 resultColor = texelFetch(color, me, 0).rgb;

    float depth_center = texelFetch(depth, me, 0);

    float r_neighborhood;
    vec2 v_neighborhood = readAdjustedNeighborhoodVelocity(me, r_neighborhood);

    if(r_neighborhood <= 0.1)
    {
        fragColor = vec4(resultColor, 1.0);
        return;
    }
        
    float radius_center;
    vec2 velocity_center = readAdjustedVelocity(me, radius_center);

    float varianceThreshold = 1.5;

    vec2 w_neighborhood = normalize(v_neighborhood);
    vec2 w_center = (radius_center < varianceThreshold) ? w_neighborhood : normalize(velocity_center);

    float invRadius_center = 1.0 / radius_center;
    float totalCoverage = (numSamples / 40.0) * invRadius_center;
    resultColor *= totalCoverage;

    for(int i = 0; i < numSamples; ++i)
    {
        //SMOOTHER ??
        //Do Something -> continue;

        float t = clamp( mix( -1.0, 1.0, (float(i) + 1.0 + jitter) / numSamples + 1.0) * 1.2, -1, 1);
        float dist = t * r_neighborhood;

        vec2 offset = dist * (((i & 1) == 1 ) ? w_center : w_neighborhood);
        ivec2 other = ivec2(offset + vec2(me) + vec2(0.5));

        float depth_sample = texelFetch(depth, other, 0).x;

        float radius_sample;

        vec2 velocity_sample = readAdjustedVelocity(other, radius_sample);
        vec3 color_sample = texelFetch(color, clamp(other, ivec2(0), ivec2(viewport)), 0).rgb;

        float coverage_sample = 0.0;

        float inFront = softDepthCompare(depth_center, depth_sample);
        float inBack = softDepthCompare(depth_sample, depth_center);

        coverage_sample += inFront * cone(dist, radius_sample);
        coverage_sample += inBack * fastCone(dist, invRadius_center);
        coverage_sample += cylinder(dist, min(radius_center, radius_sample)) * 2.0;

        totalCoverage += coverage_sample;
        resultColor += color_sample * coverage_sample;
    }

    fragColor = vec4(resultColor / totalCoverage, 1.0);
    fragColor = vec4(vec3(texture(velocity, v_uv)), 1.0);
}

//===============================

/*
float cone(vec2 X, vec2 Y, vec2 v)
{
    return clamp(1 - length(X - Y) / length(v), 0.0, 1.0);
}

float cylinder(vec2 X, vec2 Y, vec2 v)
{
    return 1.0 - smoothstep(0.95 * length(v), 1.05* length(v), length(X - Y));
}

float softDepthCompare(float za, float zb)
{
    return clamp( 1 - (za - zb) / 10, 0.0, 1.0);
}



void main()
{
    float blurScale = float(currentFPS) / float(targetFPS);

    vec2 X = v_uv;
    vec2 v = texture(neighborMax, v_uv / radius).rg;
    if(length(v) < length(1/viewport) * 0.9)
    {
        fragColor = texture(ssao, X);
        return;
    }

    float weight = 1.0 / texture(velocity, X);
    vec4 sum = texture(ssao, X) * weight;

    float j = noise1(X) * 0.5;

    for(int i = 0; i < numSamples; i++)
    {
        if(i != ((numSamples-1)/2))
        {
            float t = mix(-1.0, 1.0, (i + j + 1.0)/(numSamples + 1.0));
        vec2 Y = X + v * t + length(1/viewport) * 0.5;

        float f = softDepthCompare(texture(depth, X), texture(depth, Y));
        float b = softDepthCompare(texture(depth, Y), texture(depth, X));

        float alpha = f * cone(Y, X, texture(velocity, Y).rg)
                    + b * cone(X, Y, texture(velocity, X).rg)
                    + cylinder(Y, X, texture(velocity, Y).rg) * cylinder(X, Y, texture(velocity, X).rg) * 2;
        weight += alpha;
        sum += alpha * texture(ssao, Y);
        }

    }

    fragColor = sum / weight;//texture(ssao, X) * 0.5;
    //fragColor = texture(velocity, v_uv);
    //fragColor = vec4(v, 0.0, 1.0);

}



void main2() {

    vec4 finalColor = texture(ssao, v_uv);

    //TexSize nicht nutzen!

    //vec2 texelSize = 1.0 / vec2(textureSize(ssao, 0));

    vec2 texCoord = v_uv;

    vec2 blurVec = texture(velocity, v_uv).rg;

   // blurVec *= step(0.1, abs(blurVec));


    float blurScale = float(currentFPS) / float(targetFPS);
    blurVec *= blurScale * 2.5;

    for(int i = 1; i < numSamples; ++i)  
    {
        vec2 offset = blurVec * (float(i) / float(numSamples - 1) - 0.5);
        vec2 offsetVelocity = texture(velocity, texCoord + offset).rg;
        offset *= step(0.001, abs(offsetVelocity));
        finalColor += texture(ssao, texCoord + offset);
    }
     
    fragColor = vec4(finalColor.xyz / float(numSamples), 1.0);

    //fragColor = vec4(texture(velocity, v_uv).rg * 0.5 + 0.5, 0.0, 1.0);

    //fragColor = vec4(texture(velocity, v_uv).rg * 0.5 + 0.5, 0.0, 1.0);

}
*/