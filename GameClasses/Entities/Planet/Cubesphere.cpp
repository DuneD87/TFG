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
#include "../../Util/FastNoiseLite.h"



// constants //////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
// ctor
// The radius is circumscribed sphere
///////////////////////////////////////////////////////////////////////////////
Cubesphere::Cubesphere(float radius, int sub, bool smooth) : radius(radius), subdivision(sub), smooth(smooth), interleavedStride(32)
{
    vertexCountPerRow = (unsigned int)pow(2, sub) + 1;
    vertexCountPerFace = vertexCountPerRow * vertexCountPerRow;
    lPoint = 0;
    hPoint = 0;
    buildVerticesSmooth();
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
    std::cout<<"Unit vertices size: "<<unitVertices.size()<<std::endl;
    // clear memory of prev arrays
    clearArrays();
    FastNoiseLite noise;
    noise.SetSeed(1337);
    noise.SetNoiseType(FastNoiseLite::NoiseType_Value);
    noise.SetFrequency(0.015f);

    noise.SetFractalType(FastNoiseLite::FractalType_None);
    noise.SetFractalOctaves(16);
    noise.SetFractalLacunarity(1.7);
    noise.SetFractalGain(1.2);
    noise.SetFractalWeightedStrength(0.7);
    noise.SetFractalPingPongStrength(3);

    noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Hybrid);
    noise.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue);
    noise.SetCellularJitter(1.0f);
    noise.SetDomainWarpType(FastNoiseLite::DomainWarpType_BasicGrid);
    noise.SetDomainWarpAmp(50.0f);
    noise.SetFrequency(0.01f);
    noise.SetFractalType(FastNoiseLite::FractalType_DomainWarpProgressive);
    noise.SetFractalOctaves(5);
    noise.SetFractalLacunarity(2.0f);
    noise.SetFractalGain(0.60f);
    float height = 10;
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
            float noiseLevel = noise.GetNoise(x*radius,y*radius,z*radius) * height;
            if (noiseLevel > hPoint)
                hPoint = noiseLevel;
            if (noiseLevel < lPoint)
                lPoint = noiseLevel;
            glm::vec3 dir = glm::normalize(glm::vec3(0) - vertexPos);
            auxVertex.Position = vertexPos+dir*noiseLevel;
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
    startIndex = vertices.size() / 3;
    for(int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        Vertex auxVertex;
        glm::vec3 vertexPos = glm::vec3(-vertices[i], vertices[i+1], -vertices[i+2]);
        float noiseLevel = noise.GetNoise(-vertices[i], vertices[i+1], -vertices[i+2]) * height;
        if (noiseLevel > hPoint)
            hPoint = noiseLevel;
        if (noiseLevel < lPoint)
            lPoint = noiseLevel;
        glm::vec3 dir = glm::normalize(glm::vec3(0) - vertexPos);
        auxVertex.Position = vertexPos+dir*noiseLevel;
        auxVertex.Normal = glm::vec3(-normals[i], normals[i+1], -normals[i+2]);
        auxVertex.TexCoords = glm::vec2(texCoords[j], texCoords[j+1]);
        vertexList.push_back(auxVertex);
        addVertex(-vertices[i], vertices[i+1], -vertices[i+2]);
        addTexCoord(texCoords[j], texCoords[j+1]);
        addNormal(-normals[i], normals[i+1], -normals[i+2]);
    }
    for(int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }
    // build +Y face by swapping x=>y, y=>-z, z=>-x
    startIndex = vertices.size() / 3;
    for(int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        Vertex auxVertex;
        glm::vec3 vertexPos = glm::vec3(-vertices[i+2], vertices[i], -vertices[i+1]);
        float noiseLevel = noise.GetNoise(-vertices[i+2], vertices[i], -vertices[i+1]) * height;
        if (noiseLevel > hPoint)
            hPoint = noiseLevel;
        if (noiseLevel < lPoint)
            lPoint = noiseLevel;
        glm::vec3 dir = glm::normalize(glm::vec3(0) - vertexPos);
        auxVertex.Position = vertexPos+dir*noiseLevel;
        auxVertex.Normal = glm::vec3(-normals[i+2], normals[i], -normals[i+1]);
        auxVertex.TexCoords = glm::vec2(texCoords[j], texCoords[j+1]);
        vertexList.push_back(auxVertex);

        addVertex(-vertices[i+2], vertices[i], -vertices[i+1]);
        addTexCoord(texCoords[j], texCoords[j+1]);
        addNormal(-normals[i+2], normals[i], -normals[i+1]);
    }
    for(int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }

    // build -Y face by swapping x=>-y, y=>z, z=>-x
    startIndex = vertices.size() / 3;
    for(int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        Vertex auxVertex;
        glm::vec3 vertexPos = glm::vec3(-vertices[i+2], -vertices[i], vertices[i+1]);
        float noiseLevel = noise.GetNoise(-vertices[i+2], -vertices[i], vertices[i+1]) * height;
        if (noiseLevel > hPoint)
            hPoint = noiseLevel;
        if (noiseLevel < lPoint)
            lPoint = noiseLevel;
        glm::vec3 dir = glm::normalize(glm::vec3(0) - vertexPos);
        auxVertex.Position = vertexPos+dir*noiseLevel;

        auxVertex.Normal = glm::vec3(-normals[i+2], -normals[i], normals[i+1]);
        auxVertex.TexCoords = glm::vec2(texCoords[j], texCoords[j+1]);
        vertexList.push_back(auxVertex);

        addVertex(-vertices[i+2], -vertices[i], vertices[i+1]);
        addTexCoord(texCoords[j], texCoords[j+1]);
        addNormal(-normals[i+2], -normals[i], normals[i+1]);
    }
    for(int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }

    // build +Z face by swapping x=>z, z=>-x
    startIndex = vertices.size() / 3;
    for(int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        Vertex auxVertex;
        glm::vec3 vertexPos = glm::vec3(-vertices[i+2], vertices[i+1], vertices[i]);
        float noiseLevel = noise.GetNoise(-vertices[i+2], vertices[i+1], vertices[i]) * height;
        if (noiseLevel > hPoint)
            hPoint = noiseLevel;
        if (noiseLevel < lPoint)
            lPoint = noiseLevel;
        glm::vec3 dir = glm::normalize(glm::vec3(0) - vertexPos);
        auxVertex.Position = vertexPos+dir*noiseLevel;
        auxVertex.Normal = glm::vec3(-normals[i+2], normals[i+1], normals[i]);
        auxVertex.TexCoords = glm::vec2(texCoords[j], texCoords[j+1]);
        vertexList.push_back(auxVertex);

        addVertex(-vertices[i+2], vertices[i+1], vertices[i]);
        addTexCoord(texCoords[j], texCoords[j+1]);
        addNormal(-normals[i+2], normals[i+1], normals[i]);
    }
    for(int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }
    // build -Z face by swapping x=>-z, z=>x
    startIndex = vertices.size() / 3;
    for(int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        Vertex auxVertex;
        glm::vec3 vertexPos = glm::vec3(vertices[i+2], vertices[i+1], -vertices[i]);
        float noiseLevel = noise.GetNoise(vertices[i+2], vertices[i+1], -vertices[i]) * height;
        if (noiseLevel > hPoint)
            hPoint = noiseLevel;
        if (noiseLevel < lPoint)
            lPoint = noiseLevel;
        glm::vec3 dir = glm::normalize(glm::vec3(0) - vertexPos);
        auxVertex.Position = vertexPos+dir*noiseLevel;
        auxVertex.Normal = glm::vec3(normals[i+2], normals[i+1], -normals[i]);
        auxVertex.TexCoords = glm::vec2(texCoords[j], texCoords[j+1]);
        vertexList.push_back(auxVertex);

        addVertex(vertices[i+2], vertices[i+1], -vertices[i]);
        addTexCoord(texCoords[j], texCoords[j+1]);
        addNormal(normals[i+2], normals[i+1], -normals[i]);
    }
    for(int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }
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
        Vertex auxVert;
        interleavedVertices.push_back(vertices[i]);
        interleavedVertices.push_back(vertices[i+1]);
        interleavedVertices.push_back(vertices[i+2]);
        auxVert.Position = glm::vec3(vertices[i],vertices[i+1],vertices[i+2]);
        interleavedVertices.push_back(normals[i]);
        interleavedVertices.push_back(normals[i+1]);
        interleavedVertices.push_back(normals[i+2]);
        auxVert.Normal = glm::vec3(normals[i],normals[i+1],normals[i+2]);
        interleavedVertices.push_back(texCoords[j]);
        interleavedVertices.push_back(texCoords[j+1]);
        auxVert.TexCoords = glm::vec2(texCoords[j],texCoords[j+1]);
        vertexList.push_back(auxVert);
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
    }
}

float Cubesphere::getLPoint() const {
    return lPoint;
}

float Cubesphere::getHPoint() const {
    return hPoint;
}
