#version 410

uniform mat4 viewProjection;
uniform mat4 prevViewProjection;
uniform mat4 model;
uniform mat4 prevModel;
uniform mat4 view;

in vec3 a_vertex;
in vec3 a_normal;

flat out vec3 v_normal;
flat out vec3 v_color;
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
    v_color = clamp(vec3(0.4) + 
                    light(vec3(1.0, 0.3, 0.0),
                        vec3(1.0, -1.0, 1.0),
                        a_normal) +
                    light(vec3(0.5, 0.8, 0.5), 
                        vec3(-1.0,-1.0, 1.0),
                        a_normal) +
                    light(vec3(0.5, 0.8, 0.5),
                        vec3(0.0, 1.0, 0.0),
                        a_normal),
                    0.0, 1.0);

    vec4 worldVertex = model * vec4(a_vertex, 1.0);

    v_normal = vec3(view * model * vec4(a_normal, 0.0));

    vec4 vertexInVS = view * worldVertex;
    v_depthInES = vertexInVS.z / vertexInVS.w;

    v_currentPositionInWS = worldVertex;
    v_previousPositionInWS = prevModel * vec4(a_vertex, 1.0);

    gl_Position = viewProjection  * worldVertex;
}
