#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec4 worldPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 _viewPos;

uniform vec3 viewPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float pRadius;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    worldPos = projection * view * vec4(FragPos, 1.0);
    TexCoords = aTexCoords;
    gl_Position = worldPos;

}