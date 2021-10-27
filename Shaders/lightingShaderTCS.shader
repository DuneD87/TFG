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

// attributes of the output CPs
out patch OutputPatch oPatch;

float GetTessLevel(float Distance0, float Distance1)
{
    float AvgDistance = (Distance0 + Distance1) / 2.0;
    if (AvgDistance <= 2000.0)
        return 16.0;
    if (AvgDistance <= 4000.0)
        return 8.0;
    else if (AvgDistance <= 8000.0)
        return 4.0;
    else if (AvgDistance <= 16000.0)
        return 2.0;
    else
        return 1.0;

}
vec3 ProjectToPlane(vec3 Point, vec3 PlanePoint, vec3 PlaneNormal)
{
    vec3 v = Point - PlanePoint;
    float Len = dot(v, PlaneNormal);
    vec3 d = Len * PlaneNormal;
    return (Point - d);
}
void CalcPositions()
{
    // The original vertices stay the same
    oPatch.WorldPos_B030 = FragPos[0];
    oPatch.WorldPos_B003 = FragPos[1];
    oPatch.WorldPos_B300 = FragPos[2];

    // Edges are names according to the opposing vertex
    vec3 EdgeB300 = oPatch.WorldPos_B003 - oPatch.WorldPos_B030;
    vec3 EdgeB030 = oPatch.WorldPos_B300 - oPatch.WorldPos_B003;
    vec3 EdgeB003 = oPatch.WorldPos_B030 - oPatch.WorldPos_B300;

    // Generate two midpoints on each edge
    oPatch.WorldPos_B021 = oPatch.WorldPos_B030 + EdgeB300 / 3.0;
    oPatch.WorldPos_B012 = oPatch.WorldPos_B030 + EdgeB300 * 2.0 / 3.0;
    oPatch.WorldPos_B102 = oPatch.WorldPos_B003 + EdgeB030 / 3.0;
    oPatch.WorldPos_B201 = oPatch.WorldPos_B003 + EdgeB030 * 2.0 / 3.0;
    oPatch.WorldPos_B210 = oPatch.WorldPos_B300 + EdgeB003 / 3.0;
    oPatch.WorldPos_B120 = oPatch.WorldPos_B300 + EdgeB003 * 2.0 / 3.0;

    // Project each midpoint on the plane defined by the nearest vertex and its normal
    oPatch.WorldPos_B021 = ProjectToPlane(oPatch.WorldPos_B021, oPatch.WorldPos_B030,
    oPatch.Normal[0]);
    oPatch.WorldPos_B012 = ProjectToPlane(oPatch.WorldPos_B012, oPatch.WorldPos_B003,
    oPatch.Normal[1]);
    oPatch.WorldPos_B102 = ProjectToPlane(oPatch.WorldPos_B102, oPatch.WorldPos_B003,
    oPatch.Normal[1]);
    oPatch.WorldPos_B201 = ProjectToPlane(oPatch.WorldPos_B201, oPatch.WorldPos_B300,
    oPatch.Normal[2]);
    oPatch.WorldPos_B210 = ProjectToPlane(oPatch.WorldPos_B210, oPatch.WorldPos_B300,
    oPatch.Normal[2]);
    oPatch.WorldPos_B120 = ProjectToPlane(oPatch.WorldPos_B120, oPatch.WorldPos_B030,
    oPatch.Normal[0]);

    // Handle the center
    vec3 Center = (oPatch.WorldPos_B003 + oPatch.WorldPos_B030 + oPatch.WorldPos_B300) / 3.0;
    oPatch.WorldPos_B111 = (oPatch.WorldPos_B021 + oPatch.WorldPos_B012 + oPatch.WorldPos_B102 +
    oPatch.WorldPos_B201 + oPatch.WorldPos_B210 + oPatch.WorldPos_B120) / 6.0;
    oPatch.WorldPos_B111 += (oPatch.WorldPos_B111 - Center) / 2.0;
}

void main()
{
    // Set the control points of the output patch
    oPatch.Normal[gl_InvocationID] = Normal[gl_InvocationID];
    oPatch.TexCoord[gl_InvocationID] = TexCoords[gl_InvocationID];

    tbn_ES_in[gl_InvocationID] = tbn[gl_InvocationID];
    viewPos_ES_in[gl_InvocationID] = _viewPos[gl_InvocationID];
    // Calculate the distance from the camera to the three control points

    CalcPositions();
    float EyeToVertexDistance0 = distance(_viewPos[0], FragPos[0]);
    float EyeToVertexDistance1 = distance(_viewPos[1], FragPos[1]);
    float EyeToVertexDistance2 = distance(_viewPos[2], FragPos[2]);
    // Calculate the tessellation levels
    gl_TessLevelOuter[0] = GetTessLevel(EyeToVertexDistance1, EyeToVertexDistance2);
    gl_TessLevelOuter[1] = GetTessLevel(EyeToVertexDistance2, EyeToVertexDistance0);
    gl_TessLevelOuter[2] = GetTessLevel(EyeToVertexDistance0, EyeToVertexDistance1);
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}