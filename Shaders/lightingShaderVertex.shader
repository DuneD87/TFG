#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec4 worldPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;
out vec3 LocalPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out mat4 m;
out mat4 v;

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
    TexCoords = aTexCoords;

}