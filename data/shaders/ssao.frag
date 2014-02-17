#version 410

/*******************************************************************************
    Copyright (C) 2013 John Chapman

    This software is distributed freely under the terms of the MIT License.
    See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/

uniform sampler2D normal_depth;
uniform sampler2D noise;
uniform float farPlane;

uniform mat4 projection;

const int MAX_KERNEL_SIZE = 128;
uniform int kernelSize;
uniform vec3 kernel[MAX_KERNEL_SIZE];

layout (location = 0) out float fragColor;

smooth in vec2 v_uv;
smooth in vec2 v_noiseUv;
smooth in vec3 v_viewRay;

float radius = 10.0;
float uPower = 5.0;

float ssao(in mat3 kernelBasis, in vec3 originPos, in float radius) 
{
    float occlusion = 0.0;
    for (int i = 0; i < kernelSize; ++i) {
        //get sample position in ES:
        vec3 samplePos = kernelBasis * kernel[i];
        samplePos = samplePos * radius + originPos;
        
        //project sample position to SS
        vec4 offset = projection * vec4(samplePos, 1.0);
        offset.xy /= offset.w; // only need xy
        offset.xy = offset.xy * 0.5 + 0.5; // scale/bias to texcoords
        
        //get sample depth:
        float sampleDepth = texture(normal_depth, offset.xy).a;

        //do not occlude if range check is zero
        // float rangeCheck = smoothstep(0.0, 1.0, radius / (abs(originPos.z - sampleDepth)));
        occlusion += step(samplePos.z, sampleDepth);
    }
    occlusion = 1 - (occlusion / float(kernelSize));
    return pow(occlusion, uPower);
}

void main() 
{    
    //get view space origin:
    float originDepth = texture(normal_depth, v_uv).a;
    originDepth = - originDepth / farPlane;
    vec3 originPos = v_viewRay * originDepth;

    //get view space normal:
    vec3 normal = normalize(texture(normal_depth, v_uv).rgb);

    //construct kernel basis matrix:
    vec3 rvec = normalize(texture(noise, v_noiseUv).rgb);
    vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
    vec3 bitangent = cross(tangent, normal);
    mat3 kernelBasis = mat3(tangent, bitangent, normal);
    
    fragColor = ssao(kernelBasis, originPos, sqrt(originDepth) * radius);
}
