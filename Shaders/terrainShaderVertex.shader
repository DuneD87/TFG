#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 LocalPos;
out vec4 worldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    LocalPos = aPos;
    Normal = aNormal;
    TexCoords = aTexCoords;
    worldPos = projection * view * vec4(FragPos, 1.0);
    gl_Position = worldPos;
}