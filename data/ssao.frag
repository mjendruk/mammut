#version 410

/*******************************************************************************
    Copyright (C) 2013 John Chapman

    This software is distributed freely under the terms of the MIT License.
    See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/

uniform sampler2D normal;
uniform sampler2D depth;
uniform sampler2D noise;

uniform mat3 normalMatrix;

layout (location = 0) out vec4 fragColor;

smooth in vec2 v_uv;
smooth in vec3 v_eyevector;

uniform mat4 projection; // current projection matrix, for linearized depth
uniform mat4 viewProjectionInv;

//	ssao uniforms:
const int MAX_KERNEL_SIZE = 128;
uniform int kernelSize;
uniform vec3 kernel[MAX_KERNEL_SIZE];
float radius = 0.07;
float uPower = 2.0;


void main2()
{
	fragColor = vec4(texture(depth, v_uv) * 0.5 + 0.5);
	float originDepth = texture(depth, v_uv).r;
	fragColor = vec4(v_eyevector, 1.0);
	//fragColor = vec4(kernel[6], 1.0);
}

//v_eyevector falschrum?


/*
in VertexDataT {
	smooth vec3 position;
	smooth vec2 texcoord; // required
	smooth mat3 tangentViewMatrix;
	noperspective vec3 viewRay; // required
#ifdef VELOCITY_BUFFER
	smooth vec4 projPosition;
	smooth vec4 prevProjPosition;
#endif
} vertexIn;
*/
float linearizeDepth(in float depth, in mat4 projMatrix) {
	return projMatrix[3][2] / (depth - projMatrix[2][2]);
}
/*----------------------------------------------------------------------------*/
float ssao(in mat3 kernelBasis, in vec3 originPos, in float radius) {
	float occlusion = 0.0;
	for (int i = 0; i < kernelSize; ++i) {
	//	get sample position:
		vec3 samplePos = kernelBasis * kernel[i];
		samplePos = samplePos * radius + originPos;
		
	//	project sample position:
		vec4 offset = projection * vec4(samplePos, 1.0);
		offset.xy /= offset.w; // only need xy
		offset.xy = offset.xy * 0.5 + 0.5; // scale/bias to texcoords
		
	//	get sample depth:
		float sampleDepth = texture(depth, offset.xy).r;
		sampleDepth = linearizeDepth(sampleDepth, projection);
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(originPos.z - sampleDepth));
		occlusion += rangeCheck * step(sampleDepth, samplePos.z);
	}
	
	occlusion = 1.0 - (occlusion / float(kernelSize)) / 1 ;
	return pow(occlusion, uPower);
}

/*----------------------------------------------------------------------------*/
void main() {
	if(texture(depth, v_uv).r == 1.0) {
        fragColor = vec4(1.0);
        return;
    }
//	get noise texture coords:
	vec2 noiseTexCoords = vec2(textureSize(depth, 0)) / vec2(textureSize(noise, 0));
	noiseTexCoords *= v_uv;
	
//	get view space origin:
	float originDepth = texture(depth, v_uv).r;
	originDepth = linearizeDepth(originDepth, projection);
	vec3 originPos = normalize(v_eyevector * vec3(1.0, 1.0, -1.0)) * originDepth;

//	get view space normal:
	vec3 normal = normalMatrix * texture(normal, v_uv).rgb;
	normal = normalize(normal);
	fragColor = normalize(vec4(normal, 1.0));
		
//	construct kernel basis matrix:
	vec3 rvec = texture(noise, noiseTexCoords).rgb * 2.0 - 1.0;
	vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
	vec3 bitangent = cross(tangent, normal);
	mat3 kernelBasis = mat3(tangent, bitangent, normal);
	
	fragColor = vec4(ssao(kernelBasis, originPos, radius));
	//fragColor = vec4(v_eyevector, 1.0);
}



void main3()
{
    float depthValue = texture(depth, v_uv).r;
    
    if(depthValue == 1.0) {
        fragColor = vec4(1.0);
        return;
    }

    float radius2 = radius*100 / ((1500)/2.0);

    vec3 origin = vec3(v_uv, depthValue);
    vec3 normal = normalize(normalMatrix * texture(normal, v_uv).xyz * 2.0 - 1.0);
 
    // create matrix to rotate
    vec2 noiseTexCoords = vec2(textureSize(depth, 0)) / vec2(textureSize(noise, 0));
	noiseTexCoords *= v_uv;
	vec3 rvec = texture(noise, noiseTexCoords).rgb * 2.0 - 1.0;
	//fragColor = vec4(texture(noise, noiseTexCoords).rgb, 1.0);
	//return;
    vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    for (int i = 0; i < kernelSize; ++i) {
        // get sample position:
        vec3 samplePos = tbn * kernel[i];
        samplePos = samplePos * radius2 + origin;

        // project sample position: --- I dont know what the purpose of this is
        //vec4 offset = vec4(samplePos, 1.0);
        //offset = uProjectionMat * offset;
        //offset.xy /= offset.w;
        //offset.xy = offset.xy * 0.5 + 0.5;
      
        // get sample depth:
        float sampleDepth = texture(depth, samplePos.xy).r;
      
        // range check & accumulate:
        float rangeCheck = abs(origin.z - sampleDepth) < radius2 ? 1.0 : 0.0;
        occlusion += (sampleDepth <= samplePos.z - 0.005 ? 1.0 : 0.0) * rangeCheck;
    }
    fragColor = vec4(1.0 - (occlusion / kernelSize));
}
