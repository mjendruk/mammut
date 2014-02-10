#version 410

uniform sampler2D depth;
uniform sampler2D color;
uniform sampler2D velocity;
uniform sampler2D neighborMax;

uniform sampler2D randomBuffer;

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
    vec2 q = texelFetch(sampler, C, 0).xy * viewport;
    float lenq = length(q);

    r = (lenq * 0.5) * currentFPS_targetFPS;

    //HALF_PIX = 0.5
    r = clamp(r, 0.5, radius);

    return q * (r / lenq);
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
{   // coords: [0, viewport]
    ivec2 me = ivec2(gl_FragCoord.xy);
    // jitter [0 , 1] -> [-0.5, 0.5]
    float jitter = hash(me) - 0.5;

    vec3 resultColor = texelFetch(color, me, 0).rgb;

    float depth_center = texelFetch(depth, me, 0);

    // largest velocity in neighborhood of current pixel
    // r_n: [0, radius]    +   v_n: length(v_n) = r_n
    float radius_neighborhood;
    vec2 velocity_neighborhood = readAdjustedNeighborhoodVelocity(me, radius_neighborhood);

    if(radius_neighborhood <= 0.55)
    {
        fragColor = vec4(resultColor, 1.0);
        //fragColor = vec4(0.1, 0.2, 0.3, 1.0);
        return;
    }
    // veclocity and its length (radius) of current pixel (sample center)
    // radius: [0, radius]   + velocity: length(velocity) = radius
    float radius_center;
    vec2 velocity_center = readAdjustedVelocity(me, radius_center);

    float varianceThreshold = 2.5;

    // w_neighnorhood: direction of the largset velocity in neigborhood
    // length: 1
    vec2 direction_neighborhood = normalize(velocity_neighborhood);
    // direction_center: velocity direction of center (current pixel)
    // either largest neighborhood velocity direction or velocity of current pixel
    // if velocity of current pixel is to slow -> use velocity of neighbor
    // length: 1
    vec2 direction_center = (radius_center < varianceThreshold) ? direction_neighborhood : normalize(velocity_center);

    //
    float invRadius_center = 1.0 / radius_center;
    float totalCoverage = (numSamples / 55.0) * invRadius_center;
    resultColor *= totalCoverage;

    for(int i = 0; i < numSamples; ++i)
    {
        //SMOOTHER ??
        //Do Something -> continue;

        if(i == (numSamples - 1)/2)
            continue;

        // [-1, 1]
        float t = clamp( mix( -1.0, 1.0, (float(i) + 1.0 + jitter) / (numSamples + 1.0)) * 1.2, -1.0, 1.0);
        // distance (less or equal largest neighbor velocity): [-radius, radius]
        float dist = t * radius_neighborhood;
        // offset vector that will be added to current point: [-radius, radius]
        // (either direction of current point or largest neighborhood)
        vec2 offset = dist * (((i & 1) == 1 ) ? direction_center : direction_neighborhood);
        // current sample point: [0, viewport]
        ivec2 other = clamp(ivec2(offset + vec2(me) + vec2(0.5)), ivec2(0.0), ivec2(viewport));
        //depth of sample
        float depth_sample = texelFetch(depth, other, 0).x;
        // velocity length of sample: [0, radius]
        float radius_sample;
        // velocity of sample
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
    float fl;
    vec2 vec = readAdjustedNeighborhoodVelocity(me, fl);
    //fragColor = vec4(vec3(fl), 1.0);
    //fragColor = vec4(vec3(length(texelFetch(velocity, me, 0).xy / viewport)), 1.0);
    //fragColor = vec4(texture(velocity, v_uv).rgb, 1.0);
    //fragColor = vec4(vec3(vec, 0.0), 1.0);
}