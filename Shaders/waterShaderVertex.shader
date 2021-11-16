#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;
out vec4 worldPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform float a_time;
uniform float near;
uniform float far;
uniform vec3 planetOrigin;

void main()
{
    vec3 pos = aPos;

    //pos.y = (sin(2.0 * (pos.x) + a_time/1000.0 ) * cos(1.5 * (pos.y) + a_time/1000.0) * 0.2);
    FragPos = vec3(model * vec4(pos, 1.0));
    vec3 dir = normalize(FragPos - planetOrigin) * sin(a_time*1000);
    FragPos = FragPos;
    Normal = aNormal;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
    float near = 10.0;
    float far = 10000000.0;
    gl_Position.z = 2.0f * log(gl_Position.w/near)/log(far/near) - 1;
    gl_Position.z *= gl_Position.w;
}