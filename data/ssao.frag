#version 410

/*******************************************************************************
    Copyright (C) 2013 John Chapman

    This software is distributed freely under the terms of the MIT License.
    See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/

uniform sampler2D g_normal;
uniform sampler2D depth;
uniform sampler2D noise;

uniform mat3 normalMatrix;
uniform mat4 projection;
uniform mat4 viewProjectionInv;

const int MAX_KERNEL_SIZE = 128;
uniform int kernelSize;
uniform vec3 kernel[MAX_KERNEL_SIZE];

layout (location = 0) out vec4 fragColor;

smooth in vec2 v_uv;
smooth in vec3 v_eyevector;


float radius = 0.002;
float uPower = 1.2;

float ssao(in mat3 kernelBasis, in vec3 originPos, in float radius) {
    float occlusion = 0.0;
    for (int i = 0; i < kernelSize; ++i) {
        //get sample position; lets assume that originPos is in ES:
        vec3 samplePos = kernelBasis * kernel[i];
        samplePos = samplePos * radius + originPos;
        
        //project sample position:
        vec4 offset = projection * vec4(samplePos, 1.0);
        offset.xy /= offset.w; // only need xy
        offset.xy = offset.xy * 0.5 + 0.5; // scale/bias to texcoords
        
        //get sample depth:
        float sampleDepth = texture(g_normal, vec2(1.0) - offset.xy).a;

        //range check is zero if we must not occlude
        float rangeCheck = smoothstep(0.0, 1.0, radius / (abs(originPos.z - sampleDepth)));
        occlusion += rangeCheck * step(sampleDepth, samplePos.z);
    }
    occlusion = (occlusion / float(kernelSize));
    return 1 - pow(occlusion, uPower);
}

void main() {
    if(texture(depth, v_uv).r == 1.0) {
        fragColor = vec4(1.0);
        return;
    }
    //get noise texture coords:
    //todo: get rid of the textureSize lookup; use uniforms
    vec2 noiseTexCoords = vec2(textureSize(depth, 0)) / vec2(textureSize(noise, 0));
    noiseTexCoords *= v_uv;
    
    //get view space origin:
    float originDepth = texture(g_normal, v_uv).a;

    // Todo: originPos has to be in which coordinate system?; Currently its most wahrscheinlich in (noramlized) sceen coordinate system; I guess it has to be in ES;
    vec3 originPos = normalize(v_eyevector);
    originPos /= originPos.z;
    originPos *= originDepth;

    //get view space normal:
    vec3 normal = normalMatrix * texture(g_normal, v_uv).rgb;
    normal = normalize(normal);
    normal.z *= -1;
        
    //construct kernel basis matrix:
    vec3 rvec = texture(noise, noiseTexCoords).rgb * 2.0 - 1.0;
    vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
    vec3 bitangent = cross(tangent, normal);
    mat3 kernelBasis = mat3(tangent, bitangent, normal);
    
    fragColor = vec4(vec3(ssao(kernelBasis, originPos, radius)), 1.0);
}
