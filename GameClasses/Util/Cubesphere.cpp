///////////////////////////////////////////////////////////////////////////////
// Cubesphere.cpp
// ==============
// cube-based sphere dividing the spherical surface into 6 equal-area faces of
// a cube (+X, -X, +Y, -Y, +Z, -Z)
// If N=0, it is identical to a cube, which is inscribed in a sphere.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2018-09-20
// UPDATED: 2019-12-28
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <cmath>
#include "Cubesphere.h"
#include "FastNoiseLite.h"



// constants //////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
// ctor
// The radius is circumscribed sphere
///////////////////////////////////////////////////////////////////////////////
Cubesphere::Cubesphere(float radius, int sub, bool smooth, float minValue) : radius(radius), subdivision(sub), smooth(smooth), interleavedStride(32)
{
    vertexCountPerRow = (unsigned int)pow(2, sub) + 1;
    vertexCountPerFace = vertexCountPerRow * vertexCountPerRow;
    lPoint = 0;
    hPoint = 0;
    this->minValue = minValue;
}



///////////////////////////////////////////////////////////////////////////////
// setters
///////////////////////////////////////////////////////////////////////////////
void Cubesphere::setRadius(float radius)
{
    this->radius = radius;
    updateRadius(); // update vertex positions only
}


///////////////////////////////////////////////////////////////////////////////
// print itself
///////////////////////////////////////////////////////////////////////////////
void Cubesphere::printSelf() const
{

    std::cout << "===== Cubesphere =====\n"
              << "        Radius: " << radius << "\n"
              << "   Side Length: " << getSideLength() << "\n"
              << "   Subdivision: " << subdivision << "\n"
              << "    Smoothness: " << (smooth ? "true" : "false") << "\n"
              << "Triangle Count: " << getTriangleCount() << "\n"
              << "   Index Count: " << getIndexCount() << "\n"
              << "  Vertex Count: " << getVertexCount() << "\n"
              << "  Normal Count: " << getNormalCount() << "\n"
              << "TexCoord Count: " << getTexCoordCount() << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
// update vertex positions only
///////////////////////////////////////////////////////////////////////////////
void Cubesphere::updateRadius()
{
    float scale = computeScaleForLength(&vertices[0], radius);

    std::size_t i, j;
    std::size_t count = vertices.size();
    for(i = 0, j = 0; i < count; i += 3, j += 8)
    {
        vertices[i]   *= scale;
        vertices[i+1] *= scale;
        vertices[i+2] *= scale;

        // for interleaved array
        interleavedVertices[j]   *= scale;
        interleavedVertices[j+1] *= scale;
        interleavedVertices[j+2] *= scale;
    }
}

///////////////////////////////////////////////////////////////////////////////
// dealloc vectors
///////////////////////////////////////////////////////////////////////////////
void Cubesphere::clearArrays()
{
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);
}

///////////////////////////////////////////////////////////////////////////////
// generate vertices with smooth shading
///////////////////////////////////////////////////////////////////////////////
void Cubesphere::buildVerticesSmooth()
{
    // generate unit-length verties in +X face
    std::vector<float> unitVertices = Cubesphere::getUnitPositiveX(vertexCountPerRow);
    // clear memory of prev arrays
    clearArrays();

    float x, y, z, s, t;
    int k = 0, k1, k2;

    // build +X face
    for(unsigned int i = 0; i < vertexCountPerRow; ++i)
    {
        k1 = i * vertexCountPerRow;     // index for curr row
        k2 = k1 + vertexCountPerRow;    // index for next row
        t = (float)i / (vertexCountPerRow - 1) * vertexCountPerRow;

        for(unsigned int j = 0; j < vertexCountPerRow; ++j, k += 3, ++k1, ++k2)
        {
            x = unitVertices[k];
            y = unitVertices[k+1];
            z = unitVertices[k+2];

            s = ((float)j / (vertexCountPerRow - 1)) *vertexCountPerRow ;
            Vertex auxVertex;
            glm::vec3 vertexPos = glm::vec3(x*radius,y*radius,z*radius);
            float minNoise = noise.GetNoise(x*radius,y*radius,z*radius);
            float noiseLevel = minNoise * height;
            if (noiseLevel > hPoint)
                hPoint = noiseLevel;
            if (noiseLevel < lPoint)
                lPoint = noiseLevel;
            glm::vec3 dir = glm::normalize(glm::vec3(0) - vertexPos);
            auxVertex.Position = vertexPos + dir*noiseLevel;
            auxVertex.Normal = glm::vec3(x,y,z);
            auxVertex.TexCoords = glm::vec2(s,t);

            vertexList.push_back(auxVertex);
            addVertex(x*radius, y*radius, z*radius);
            addNormal(x, y, z);
            addTexCoord(s, t);

            // add indices
            if(i < (vertexCountPerRow-1) && j < (vertexCountPerRow-1))
            {
                addIndices(k1, k2, k1+1);
                addIndices(k1+1, k2, k2+1);

            }
        }
    }

    // array size and index for building next face
    unsigned int startIndex;                    // starting index for next face
    int vertexSize = (int)vertices.size();      // vertex array size of +X face
    int indexSize = (int)indices.size();        // index array size of +X face
    int lineIndexSize = (int)lineIndices.size(); // line index size of +X face

    // build -X face by negating x and z

    std::vector<int> index1 = {0,1,2};
    std::vector<Vertex> vertices0;
    std::vector<unsigned int> indices0;
    buildFace(glm::vec3(-1,1,-1), index1,vertexSize,indexSize,vertices0,indices0);;
    finishFaceComputation(vertices0);
    // build +Y face by swapping x=>y, y=>-z, z=>-x
    std::vector<int> index2 = {2,0,1};
    std::vector<Vertex> vertices1;
    std::vector<unsigned int> indices1;
    buildFace(glm::vec3(-1,1,-1), index2,vertexSize,indexSize,vertices1,indices1);
    finishFaceComputation(vertices1);
    // build -Y face by swapping x=>-y, y=>z, z=>-x
    std::vector<int> index3 = {2,0,1};
    std::vector<Vertex> vertices2;
    std::vector<unsigned int> indices2;
    buildFace(glm::vec3(-1,-1,1), index3,vertexSize,indexSize,vertices2,indices2);
    finishFaceComputation(vertices2);
    // +Z
    std::vector<Vertex> vertices3;
    std::vector<unsigned int> indices3;
    std::vector<int> index4 = {2,1,0};
    buildFace(glm::vec3(-1,1,1), index4,vertexSize,indexSize,vertices3,indices3);
    finishFaceComputation(vertices3);
    // build -Z face by swapping x=>-z, z=>x
    std::vector<int> index5 = {2,1,0};
    std::vector<Vertex> vertices4;
    std::vector<unsigned int> indices4;
    buildFace(glm::vec3(1,1,-1), index5,vertexSize,indexSize,vertices4,indices4);
    finishFaceComputation(vertices4);

    indices.insert(indices.end(),indices0.begin(),indices0.end());
    indices.insert(indices.end(),indices1.begin(),indices1.end());
    indices.insert(indices.end(),indices2.begin(),indices2.end());
    indices.insert(indices.end(),indices3.begin(),indices3.end());
    indices.insert(indices.end(),indices4.begin(),indices4.end());



    // generate interleaved vertex array
    buildInterleavedVertices();
    computeNormals();
}



///////////////////////////////////////////////////////////////////////////////
// generate interleaved vertices: V/N/T
// stride must be 32 bytes
///////////////////////////////////////////////////////////////////////////////
void Cubesphere::buildInterleavedVertices()
{
    std::vector<float>().swap(interleavedVertices);

    std::size_t i, j;
    std::size_t count = vertices.size();
    for(i = 0, j = 0; i < count; i += 3, j += 2)
    {
        Vertex auxVertex;
        interleavedVertices.push_back(vertices[i]);
        interleavedVertices.push_back(vertices[i+1]);
        interleavedVertices.push_back(vertices[i+2]);
        float x = vertices[i];
        float y = vertices[i+1];
        float z = vertices[i+2];
        glm::vec3 vertexPos = glm::vec3(x*radius,y*radius,z*radius);
        float minNoise = noise.GetNoise(x*radius,y*radius,z*radius);
        float noiseLevel = minNoise * height;
        if (noiseLevel > hPoint)
            hPoint = noiseLevel;
        if (noiseLevel < lPoint)
            lPoint = noiseLevel;
        glm::vec3 dir = glm::normalize(glm::vec3(0) - vertexPos);
        auxVertex.Position = vertexPos + dir*noiseLevel;


        interleavedVertices.push_back(normals[i]);
        interleavedVertices.push_back(normals[i+1]);
        interleavedVertices.push_back(normals[i+2]);
        auxVertex.Normal = glm::vec3(normals[i],normals[i+1],normals[i+2]);

        interleavedVertices.push_back(texCoords[j]);
        interleavedVertices.push_back(texCoords[j+1]);
        auxVertex.TexCoords = glm::vec2(texCoords[j],texCoords[j+1]);
        vertexList.push_back(auxVertex);
    }
}



///////////////////////////////////////////////////////////////////////////////
// add single vertex to array
///////////////////////////////////////////////////////////////////////////////
void Cubesphere::addVertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

///////////////////////////////////////////////////////////////////////////////
// add single normal to array
///////////////////////////////////////////////////////////////////////////////
void Cubesphere::addNormal(float nx, float ny, float nz)
{
    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}

///////////////////////////////////////////////////////////////////////////////
// add single texture coord to array
///////////////////////////////////////////////////////////////////////////////
void Cubesphere::addTexCoord(float s, float t)
{
    texCoords.push_back(s);
    texCoords.push_back(t);
}

///////////////////////////////////////////////////////////////////////////////
// add 3 indices to array
///////////////////////////////////////////////////////////////////////////////
void Cubesphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

///////////////////////////////////////////////////////////////////////////////
// get the scale factor for vector to resize to the given length of vector
///////////////////////////////////////////////////////////////////////////////
float Cubesphere::computeScaleForLength(const float v[3], float length)
{
    // and normalize the vector then re-scale to new radius
    return length / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}



///////////////////////////////////////////////////////////////////////////////
// generate vertices for +X face only by intersecting 2 circular planes
// (longitudinal and latitudinal) at the longitude/latitude angles
///////////////////////////////////////////////////////////////////////////////
std::vector<float> Cubesphere::getUnitPositiveX(unsigned int pointsPerRow)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    std::vector<float> vertices;
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along y-axis
    float a2;           // latitudinal angle
    float scale;

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis
    for(unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis
        for(unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            scale = Cubesphere::computeScaleForLength(v, 1);

            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);

            // DEBUG
            //std::cout << "vertex: (" << v[0] << ", " << v[1] << ", " << v[2] << "), "
            //          << sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]) << std::endl;
        }
    }

    return vertices;
}

void Cubesphere::computeNormals() {
    //std::vector<std::vector<Mesh>*> diamonds;
    //std::vector<Mesh> *triangle = new std::vector<Mesh>;
    int count = 0;
    for (int i = 0; i < indices.size();i+=3)
    {
        unsigned int iA = indices[i];
        unsigned int iB = indices[i+1];
        unsigned int iC = indices[i+2];
        auto edgeAB = vertexList[iB].Position - vertexList[iA].Position;
        auto edgeAC = vertexList[iC].Position - vertexList[iA].Position;
        auto norm = glm::normalize(glm::cross(edgeAB,edgeAC));
        vertexList[iA].Normal = norm;
        vertexList[iB].Normal = norm;
        vertexList[iC].Normal = norm;

        glm::vec2 deltaUV1 = vertexList[iB].TexCoords - vertexList[iA].TexCoords;
        glm::vec2 deltaUV2 = vertexList[iC].TexCoords - vertexList[iA].TexCoords;

        glm::vec3 tangent,bitangent;
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent.x = f * (deltaUV2.y * edgeAB.x - deltaUV1.y * edgeAC.x);
        tangent.y = f * (deltaUV2.y * edgeAB.y - deltaUV1.y * edgeAC.y);
        tangent.z = f * (deltaUV2.y * edgeAB.z - deltaUV1.y * edgeAC.z);

        bitangent.x = f * (-deltaUV2.x * edgeAB.x + deltaUV1.x * edgeAC.x);
        bitangent.y = f * (-deltaUV2.x * edgeAB.y + deltaUV1.x * edgeAC.y);
        bitangent.z = f * (-deltaUV2.x * edgeAB.z + deltaUV1.x * edgeAC.z);

        vertexList[iA].Tangent = tangent;
        vertexList[iB].Tangent = tangent;
        vertexList[iC].Tangent = tangent;

        vertexList[iA].Bitangent = bitangent;
        vertexList[iB].Bitangent = bitangent;
        vertexList[iC].Bitangent = bitangent;
    }

}

float Cubesphere::getLPoint() const {
    return lPoint;
}

float Cubesphere::getHPoint() const {
    return hPoint;
}

void Cubesphere::setupNoise(float height, FastNoiseLite noise, float minValue) {
    clearArrays();
    this->noise = noise;
    this->height = height;
    this->minValue = minValue;
    buildVerticesSmooth();
}

void Cubesphere::buildFace(glm::vec3 faceOrientation, std::vector<int> index, int vertexSize, int indexSize
                           ,std::vector<Vertex> &vList, std::vector<unsigned int> &iList) {

    int startIndex = vertices.size() / 3;
    for(int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        Vertex auxVertex;
        glm::vec3 vertexPos = glm::vec3(faceOrientation[0] * vertices[i + index[0]], faceOrientation[1] * vertices[i + index[1]], faceOrientation[2] *vertices[i + index[2]]);
        float noiseLevel = noise.GetNoise(vertexPos.x,vertexPos.y,vertexPos.z) * height;
        if (noiseLevel > hPoint)
            hPoint = noiseLevel;
        if (noiseLevel < lPoint)
            lPoint = noiseLevel;
        glm::vec3 dir = glm::normalize(glm::vec3(0) - vertexPos);
        auxVertex.Position = vertexPos+dir*noiseLevel;
        auxVertex.Normal = glm::vec3(faceOrientation[0] * normals[i + index[0]], faceOrientation[1] * normals[i + index[1]], faceOrientation[2] * normals[i + index[2]]);
        auxVertex.TexCoords = glm::vec2(texCoords[j], texCoords[j+1]);
        vList.push_back(auxVertex);

    }
    for(int i = 0; i < indexSize; ++i)
    {
        iList.push_back(startIndex + indices[i]);
    }
}

void Cubesphere::finishFaceComputation(std::vector<Vertex> verticesn) {
    for (int i = 0; i < verticesn.size();i++)
    {
        vertexList.push_back(verticesn[i]);
        addVertex(verticesn[i].Position.x,verticesn[i].Position.y,verticesn[i].Position.z);
        addTexCoord(verticesn[i].TexCoords.x, verticesn[i].TexCoords.y);
        addNormal(verticesn[i].Normal.x, verticesn[i].Normal.y, verticesn[i].Normal.z);
    }
}


