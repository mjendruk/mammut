#version 410

/**
 * taken from http://horde3d.org/wiki/index.php5?title=Shading_Technique_-_FXAA
 */

uniform sampler2D buf0;
smooth in vec2 v_uv;

layout (location = 0) out vec3 fragColor;


void main( void ) {
        //gl_FragColor.xyz = texture(buf0,v_uv).xyz;
        //return;

        ivec2 frameBufSize = textureSize(buf0, 0);

        float FXAA_SPAN_MAX = 8.0;
        float FXAA_REDUCE_MUL = 1.0/8.0;
        float FXAA_REDUCE_MIN = 1.0/128.0;

        vec3 rgbNW=texelFetch(buf0,ivec2(gl_FragCoord.xy)+ivec2(-1,-1), 0).xyz;
        vec3 rgbNE=texelFetch(buf0,ivec2(gl_FragCoord.xy)+ivec2(1,-1), 0).xyz;
        vec3 rgbSW=texelFetch(buf0,ivec2(gl_FragCoord.xy)+ivec2(-1,1), 0).xyz;
        vec3 rgbSE=texelFetch(buf0,ivec2(gl_FragCoord.xy)+ivec2(1,1), 0).xyz;
        vec3 rgbM=texelFetch(buf0,ivec2(gl_FragCoord.xy), 0).xyz;
        
        vec3 luma=vec3(0.299, 0.587, 0.114);
        float lumaNW = dot(rgbNW, luma);
        float lumaNE = dot(rgbNE, luma);
        float lumaSW = dot(rgbSW, luma);
        float lumaSE = dot(rgbSE, luma);
        float lumaM  = dot(rgbM,  luma);
        
        float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
        float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
        
        vec2 dir;
        dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
        dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
        
        float dirReduce = max(
                (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
                FXAA_REDUCE_MIN);
          
        float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
        
        dir = min(vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),
                  max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
                  dir * rcpDirMin)) / frameBufSize;
                
        vec3 rgbA = (1.0/2.0) * (
                texture(buf0, v_uv.xy + dir * (1.0/3.0 - 0.5)).xyz +
                texture(buf0, v_uv.xy + dir * (2.0/3.0 - 0.5)).xyz);
        vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
                texture(buf0, v_uv.xy + dir * (0.0/3.0 - 0.5)).xyz +
                texture(buf0, v_uv.xy + dir * (3.0/3.0 - 0.5)).xyz);
        float lumaB = dot(rgbB, luma);

        if((lumaB < lumaMin) || (lumaB > lumaMax)){
                fragColor=rgbA;
        }else{
                fragColor=rgbB;
        }
}
