#version 410 core

// define the number of CPs in the output patch
layout (vertices = 3) out;

in vec3 _viewPos[];

// attributes of the input CPs
in vec3 FragPos[];
in vec2 TexCoords[];
in vec3 Normal[];
in mat3 tbn[];

// attributes of the output CPs
out vec3 WorldPos_ES_in[];
out vec2 TexCoord_ES_in[];
out vec3 Normal_ES_in[];
out vec3 viewPos_ES_in[];
out mat3 tbn_ES_in[];

float GetTessLevel(float Distance0, float Distance1)
{
    float AvgDistance = (Distance0 + Distance1) / 2.0;
    if (AvgDistance <= 500.0)
        return 16.0;
    if (AvgDistance <= 2000.0)
        return 4.0;
    else if (AvgDistance <= 4000.0)
        return 3.0;
    else if (AvgDistance <= 10000.0)
        return 2.0;
    else
        return 1.0;

}

void main()
{
    // Set the control points of the output patch
    TexCoord_ES_in[gl_InvocationID] = TexCoords[gl_InvocationID];
    Normal_ES_in[gl_InvocationID] = Normal[gl_InvocationID];
    WorldPos_ES_in[gl_InvocationID] = FragPos[gl_InvocationID];
    tbn_ES_in[gl_InvocationID] = tbn[gl_InvocationID];
    viewPos_ES_in[gl_InvocationID] = _viewPos[gl_InvocationID];
    // Calculate the distance from the camera to the three control points
    float EyeToVertexDistance0 = distance(_viewPos[0], WorldPos_ES_in[0]);
    float EyeToVertexDistance1 = distance(_viewPos[1], WorldPos_ES_in[1]);
    float EyeToVertexDistance2 = distance(_viewPos[2], WorldPos_ES_in[2]);

    // Calculate the tessellation levels
    gl_TessLevelOuter[0] = GetTessLevel(EyeToVertexDistance1, EyeToVertexDistance2);
    gl_TessLevelOuter[1] = GetTessLevel(EyeToVertexDistance2, EyeToVertexDistance0);
    gl_TessLevelOuter[2] = GetTessLevel(EyeToVertexDistance0, EyeToVertexDistance1);
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}