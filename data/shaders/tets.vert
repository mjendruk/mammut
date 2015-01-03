#version 410

uniform mat4 viewProjection;
uniform mat4 prevViewProjection;
uniform mat4 model;
uniform mat4 prevModel;
uniform mat4 view;

uniform samplerBuffer vertices;
uniform samplerBuffer normals;
uniform samplerBuffer matrices;

in vec3 a_vertex;

flat out vec3 v_normal;
flat out vec4 v_color;
smooth out float v_depthInES;

smooth out vec4 v_currentPositionInWS;
smooth out vec4 v_previousPositionInWS;

vec3 normalToWorldSpace(vec3 normal)
{
    vec3 transformed = vec3(model * vec4(normal, 0.0));
    return normalize(transformed);
}

vec3 light(vec3 lightColor, vec3 lightNormal, vec3 vertexNormal)
{
    return dot(normalToWorldSpace(vertexNormal),
               normalize(lightNormal)) 
           * lightColor;
}

void main()
{
    vec3 normalFromTexture = texelFetch(normals, gl_InstanceID * 3 + gl_VertexID).xyz;
    vec3 vertexFromTexture = texelFetch(vertices, gl_InstanceID * 3 + gl_VertexID).xyz;
    mat4 matrixFromTexture = mat4(1.0);
    matrixFromTexture[0] = texelFetch(matrices, gl_InstanceID / 4 * 4 + 0); //integer division intended
    matrixFromTexture[1] = texelFetch(matrices, gl_InstanceID / 4 * 4 + 1);
    matrixFromTexture[2] = texelFetch(matrices, gl_InstanceID / 4 * 4 + 2);
    matrixFromTexture[3] = texelFetch(matrices, gl_InstanceID / 4 * 4 + 3);

    v_color = vec4(clamp(
                  vec3(0.4) + 
                  light(vec3(1.0, 0.3, 0.0),
                        vec3(1.0, -1.0, 1.0),
                        normalFromTexture) +
                  light(vec3(0.5, 0.8, 0.5), 
                        vec3(-1.0,-1.0, 1.0),
                        normalFromTexture) +
                  light(vec3(0.5, 0.8, 0.5),
                        vec3(0.0, 1.0, 0.0),
                        normalFromTexture)
              , 0.0, 1.0), 1.0);

    vec4 worldVertex = model * matrixFromTexture * vec4(vertexFromTexture, 1.0);

    v_normal = normalize(vec3(view * model * matrixFromTexture * vec4(normalFromTexture, 0.0)));

    vec4 vertexInVS = view * worldVertex;
    v_depthInES = vertexInVS.z / vertexInVS.w;

    v_currentPositionInWS = worldVertex;
    v_previousPositionInWS = prevModel * vec4(a_vertex, 1.0);

    gl_Position = viewProjection  * worldVertex;
}
