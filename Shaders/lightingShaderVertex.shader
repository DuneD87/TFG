#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec4 worldPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;
out vec3 LocalPos;
out mat3 tbn;
uniform vec3 viewPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform vec3 lightPos;
out mat4 m;
out mat4 v;

out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out vec3 _viewPos;
void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    LocalPos = aPos;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    worldPos = projection * view * vec4(FragPos, 1.0);
    gl_Position = worldPos;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    m = model;
    v = view;
    _viewPos = viewPos;
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    TangentLightPos = TBN * lightPos;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;
    TexCoords = aTexCoords;

}