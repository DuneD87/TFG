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
uniform float near;
uniform float far;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    worldPos = projection * view * vec4(FragPos, 1.0);
    TexCoords = aTexCoords;
    gl_Position = worldPos;
    /*float near = 10.0;
    float far = 10000000.0;
    gl_Position.z = 2.0f * log(gl_Position.w/near)/log(far/near) - 1;
    gl_Position.z *= gl_Position.w;*/
}