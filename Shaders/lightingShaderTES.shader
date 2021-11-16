#version 410 core

layout(triangles, equal_spacing, ccw) in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec2 TexCoord_ES_in[];
in vec3 WorldPos_ES_in[];
in vec3 LocalPos_ES_in[];
in vec3 Normal_ES_in[];
in vec3 viewPos_ES_in[];
in mat3 tbn_ES_in[];


struct OutputPatch
{
    vec3 WorldPos_B030;
    vec3 WorldPos_B021;
    vec3 WorldPos_B012;
    vec3 WorldPos_B003;
    vec3 WorldPos_B102;
    vec3 WorldPos_B201;
    vec3 WorldPos_B300;
    vec3 WorldPos_B210;
    vec3 WorldPos_B120;
    vec3 WorldPos_B111;
    vec3 Normal[3];
    vec2 TexCoord[3];
};
in patch OutputPatch oPatch;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 LocalPos;
out vec3 Normal;
out vec3 _viewPos;
out vec3 worldPos;
out mat3 tbn;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}
mat3 interpolateMat3D(mat3 v0, mat3 v1, mat3 v2)
{
    return mat3(gl_TessCoord.x) * v0 + mat3(gl_TessCoord.y) * v1 + mat3(gl_TessCoord.z) * v2;
}
void main()
{
    // Interpolate the attributes of the output vertex using the barycentric coordinates
    TexCoords = interpolate2D(oPatch.TexCoord[0], oPatch.TexCoord[1], oPatch.TexCoord[2]);
    Normal = interpolate3D(oPatch.Normal[0], oPatch.Normal[1], oPatch.Normal[2]);
    LocalPos = interpolate3D(LocalPos_ES_in[0],LocalPos_ES_in[1],LocalPos_ES_in[2]);
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float w = gl_TessCoord.z;

    float uPow3 = pow(u, 3);
    float vPow3 = pow(v, 3);
    float wPow3 = pow(w, 3);
    float uPow2 = pow(u, 2);
    float vPow2 = pow(v, 2);
    float wPow2 = pow(w, 2);

    FragPos = oPatch.WorldPos_B300 * wPow3 +
                oPatch.WorldPos_B030 * uPow3 +
                oPatch.WorldPos_B003 * vPow3 +
                oPatch.WorldPos_B210 * 3.0 * wPow2 * u +
                oPatch.WorldPos_B120 * 3.0 * w * uPow2 +
                oPatch.WorldPos_B201 * 3.0 * wPow2 * v +
                oPatch.WorldPos_B021 * 3.0 * uPow2 * v +
                oPatch.WorldPos_B102 * 3.0 * w * vPow2 +
                oPatch.WorldPos_B012 * 3.0 * u * vPow2 +
                oPatch.WorldPos_B111 * 6.0 * w * u * v;

    _viewPos = viewPos_ES_in[0];
    //tbn = interpolateMat3D(tbn_ES_in[0],tbn_ES_in[1],tbn_ES_in[2]);

    // Displace the vertex along the normal
    float near = 10.0;
    float far = 10000000.0;
    gl_Position =  projection * view * vec4(FragPos, 1.0);
    gl_Position.z = 2.0f * log(gl_Position.w/near)/log(far/near) - 1;
    gl_Position.z *= gl_Position.w;
}