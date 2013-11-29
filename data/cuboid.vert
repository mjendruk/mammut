#version 410

uniform mat4 transform;
uniform mat4 model;
uniform vec3 eye;

in vec3 a_vertex;
in vec3 a_normal;

flat out vec3 v_normal;

flat out vec4 v_color;
smooth out float v_eyeDistance;

vec3 normalToWorldSpace(vec3 normal)
{
    vec3 transformed = vec3(model * vec4(normal, 0.0));
    return normalize(transformed);
}

vec4 light(vec4 lightColor, vec3 lightNormal, vec3 vertexNormal)
{
    return dot(normalToWorldSpace(vertexNormal),
               normalize(lightNormal)) 
           * lightColor;
}

void main()
{
    v_color = clamp(
                  vec4(0.4) + 
                  light(vec4(1.0, 0.3, 0.0, 1.0),
                        vec3(1.0, -1.0, 1.0),
                        a_normal) +
                  light(vec4(0.0, 0.8, 0.5, 1.0), 
                        vec3(-1.0,-1.0, 1.0),
                        a_normal) +
                  light(vec4(0.2),
                        vec3(0.0, 1.0, 0.0),
                        a_normal)
              , 0.0, 1.0);

    vec4 worldVertex = model * vec4(a_vertex, 1.0);

    if (worldVertex.z > eye.z)
      v_eyeDistance = 0;
    else
      v_eyeDistance = distance(eye, worldVertex.xyz / worldVertex.w);

    v_normal = a_normal;

    gl_Position = transform * worldVertex;
}
