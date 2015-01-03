#version 410

uniform mat4 viewProjection;
uniform mat4 prevViewProjection;
uniform mat4 model;
uniform mat4 prevModel;
uniform mat4 view;
uniform mat3 normalMatrix;
uniform bool containsBoost;

in vec3 a_vertex;
in vec3 a_normal;

flat out vec3 v_normal;
flat out vec3 v_color;
smooth out float v_depthInES;

smooth out vec4 v_currentPositionInWS;
smooth out vec4 v_previousPositionInWS;

struct ColorNormal
{
    vec3 color;
    vec3 normal;
};

vec3 calculateColor()
{
    ColorNormal colorNormals[3];

    if (containsBoost)
    {
        colorNormals = ColorNormal[3](
            ColorNormal(vec3(0.5), vec3(1.0, 0.0, 0.0)),
            ColorNormal(vec3(0.5), vec3(0.0, 1.0, 0.0)),
            ColorNormal(vec3(0.5), vec3(0.0, 0.0, 1.0))
            );
    } 
    else
    {
        colorNormals = ColorNormal[3](
            ColorNormal(vec3(0.03, 0.55, 0.6), vec3(1.0, 0.0, 0.0)),
            ColorNormal(vec3(0.75, 0.23, 0.19), vec3(0.0, 1.0, 0.0)),
            ColorNormal(vec3(0.76, 0.6, 0.34), vec3(0.0, 0.0, 1.0))
            );
    }

    vec3 color = vec3(0.0);

    for (int i = 0; i < colorNormals.length(); ++i)
    {
        color += colorNormals[i].color * abs(dot(normalize(colorNormals[i].normal), a_normal));
    }

    return clamp(color, 0.0, 1.0);
}

void main()
{
    v_color = calculateColor();

    vec4 worldVertex = model * vec4(a_vertex, 1.0);

    v_normal = vec3(view * model * vec4(a_normal, 0.0));

    vec4 vertexInVS = view * worldVertex;
    v_depthInES = vertexInVS.z / vertexInVS.w;

    v_currentPositionInWS = worldVertex;
    v_previousPositionInWS = prevModel * vec4(a_vertex, 1.0);

    gl_Position = viewProjection  * worldVertex;
}
