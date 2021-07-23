//
// Created by drive on 4/7/21.
//

#ifndef OPENGLTEST_BASICSHAPEBUILDER_H
#define OPENGLTEST_BASICSHAPEBUILDER_H
#include <iostream>
#include <vector>
#include <cmath>
#include "Basic/Mesh.h"
struct Quad {
    float vertices[32] = {
            -0.5, -0.5, 0.0,   0.0, 1.0, 0.0,   0.0, 0.0,
            -0.5, 0.5, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0,
            0.5, -0.5,-0.0,   0.0, 1.0, 0.0,   1.0, 0.0,
            0.5, 0.5,0.0,   0.0, 1.0, 0.0,   1.0, 1.0
    };
    unsigned int indices[6] = {
            0, 1, 2, 1, 3, 2
    };
};
struct Cube {
    float vertices[192] = {
            -0.5, -0.5,  0.5,   0.0,  0.0,  1.0,   0.0, 0.0,
            0.5, -0.5,  0.5,   0.0,  0.0,  1.0,   1.0, 0.0,
            0.5,  0.5,  0.5,   0.0,  0.0,  1.0,   1.0, 1.0,
            -0.5,  0.5,  0.5,   0.0,  0.0,  1.0,   0.0, 1.0,
            0.5, -0.5,  0.5,   1.0,  0.0,  0.0,   0.0, 0.0,
            0.5, -0.5, -0.5,   1.0,  0.0,  0.0,   1.0, 0.0,
            0.5,  0.5, -0.5,   1.0,  0.0,  0.0,   1.0, 1.0,
            0.5,  0.5,  0.5,   1.0,  0.0,  0.0,   0.0, 1.0,
            0.5, -0.5, -0.5,   0.0,  0.0, -1.0,   0.0, 0.0,
            -0.5, -0.5, -0.5,   0.0,  0.0, -1.0,   1.0, 0.0,
            -0.5,  0.5, -0.5,   0.0,  0.0, -1.0,   1.0, 1.0,
            0.5,  0.5, -0.5,   0.0,  0.0, -1.0,   0.0, 1.0,
            -0.5, -0.5, -0.5,  -1.0,  0.0,  0.0,   0.0, 0.0,
            -0.5, -0.5,  0.5,  -1.0,  0.0,  0.0,   1.0, 0.0,
            -0.5,  0.5,  0.5,  -1.0,  0.0,  0.0,   1.0, 1.0,
            -0.5,  0.5, -0.5,  -1.0,  0.0,  0.0,   0.0, 1.0,
            -0.5,  0.5,  0.5,   0.0,  1.0,  0.0,   0.0, 0.0,
            0.5,  0.5,  0.5,   0.0,  1.0,  0.0,   1.0, 0.0,
            0.5,  0.5, -0.5,   0.0,  1.0,  0.0,   1.0, 1.0,
            -0.5,  0.5, -0.5,   0.0,  1.0,  0.0,   0.0, 1.0,
            -0.5, -0.5, -0.5,   0.0, -1.0,  0.0,   0.0, 0.0,
            0.5, -0.5, -0.5,   0.0, -1.0,  0.0,   1.0, 0.0,
            0.5, -0.5,  0.5,   0.0, -1.0,  0.0,   1.0, 1.0,
            -0.5, -0.5,  0.5,   0.0, -1.0,  0.0,   0.0, 1.0
    };
    unsigned int indices[36] = {
        0,  1,  2,  0,  2,  3,
        4,  5,  6,  4,  6,  7,
        8,  9, 10,  8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };

};

struct Cover {
    float vertices[104] = {
            1,      0,     0.0,   0.0,  0.0,  1.0,   1,     0.5,
            0.866,  0.5,   0.0,   0.0,  0.0,  1.0,   0.933, 0.75,
            0.5,    0.866, 0.0,   0.0,  0.0,  1.0,   0.75,  0.933,
            0,      1,     0.0,   0.0,  0.0,  1.0,   0.5,   1,
            -0.5,    0.866, 0.0,   0.0,  0.0,  1.0,   0.25,  0.933,
            -0.86,   0.5,   0.0,   0.0,  0.0,  1.0,   0.07,  0.75,
            -1,      0,     0.0,   0.0,  0.0,  1.0,   0,     0.5,
            -0.866, -0.5,   0.0,   0.0,  0.0,  1.0,   0.067, 0.25,
            -0.5,   -0.866, 0.0,   0.0,  0.0,  1.0,   0.25,  0.067,
            0,     -1,     0.0,   0.0,  0.0,  1.0,   0.5,   0,
            0.5,   -0.866, 0.0,   0.0,  0.0,  1.0,   0.75,  0.067,
            0.866, -0.5,   0.0,   0.0,  0.0,  1.0,   0.933, 0.25,
            0,      0,     0.0,   0.0,  0.0,  1.0,   0.5,   0.5
    };
    unsigned int indices[36] = {
            0, 1, 12, 1, 2, 12, 2, 3, 12, 3,  4, 12,  4,  5, 12,  5, 6, 12,
            6, 7, 12, 7, 8, 12, 8, 9, 12, 9, 10, 12, 10, 11, 12, 11, 0, 12
    };
};
struct Cone {
    float vertices[192] = {
            1.000,  0.000, 0.000,  0.707,  0.000, 0.707, 1.00, 0.50,
            0.000,  0.000, 1.000,  0.683,  0.183, 0.707, 0.50, 0.50,
            0.866,  0.500, 0.000,  0.612,  0.354, 0.707, 0.93, 0.75,
            0.000,  0.000, 1.000,  0.500,  0.500, 0.707, 0.50, 0.50,
            0.500,  0.866, 0.000,  0.354,  0.612, 0.707, 0.75, 0.93,
            0.000,  0.000, 1.000,  0.183,  0.683, 0.707, 0.50, 0.50,
            0.000,  1.000, 0.000,  0.000,  0.707, 0.707, 0.50, 1.00,
            0.000,  0.000, 1.000, -0.183,  0.683, 0.707, 0.50, 0.50,
            -0.500,  0.866, 0.000, -0.354,  0.612, 0.707, 0.25, 0.93,
            0.000,  0.000, 1.000, -0.500,  0.500, 0.707, 0.50, 0.50,
            -0.866,  0.500, 0.000, -0.612,  0.354, 0.707, 0.07, 0.75,
            0.000,  0.000, 1.000, -0.683,  0.183, 0.707, 0.50, 0.50,
            -1.000,  0.000, 0.000, -0.707,  0.000, 0.707, 0.00, 0.50,
            0.000,  0.000, 1.000, -0.683, -0.183, 0.707, 0.50, 0.50,
            -0.866, -0.500, 0.000, -0.612, -0.354, 0.707, 0.07, 0.25,
            0.000,  0.000, 1.000, -0.500, -0.500, 0.707, 0.50, 0.50,
            -0.500, -0.866, 0.000, -0.354, -0.612, 0.707, 0.25, 0.07,
            0.000,  0.000, 1.000, -0.183, -0.683, 0.707, 0.50, 0.50,
            0.000, -1.000, 0.000, -0.000, -0.707, 0.707, 0.50, 0.00,
            0.000,  0.000, 1.000,  0.183, -0.683, 0.707, 0.50, 0.50,
            0.500, -0.866, 0.000,  0.354, -0.612, 0.707, 0.75, 0.07,
            0.000,  0.000, 1.000,  0.500, -0.500, 0.707, 0.50, 0.50,
            0.866, -0.500, 0.000,  0.612, -0.354, 0.707, 0.93, 0.25,
            0.000,  0.000, 1.000,  0.683, -0.183, 0.707, 0.50, 0.50
    };
    unsigned int indices[36] = {
            0,  2,  1,  2,  4,  3,  4,  6,  5,  6,  8,  7,  8, 10,  9, 10, 12, 11,
            12, 14, 13, 14, 16, 15, 16, 18, 17, 18, 20, 19, 20, 22, 21, 22,  0, 23
    };
};
struct Cylinder {
    float vertices[208] = {
            1.000,  0.000, 0.000,  1.000,  0.000, 0.000, 0.000, 0.000,
            0.866,  0.500, 0.000,  0.866,  0.500, 0.000, 0.083, 0.000,
            0.500,  0.866, 0.000,  0.500,  0.866, 0.000, 0.167, 0.000,
            0.000,  1.000, 0.000,  0.000,  1.000, 0.000, 0.250, 0.000,
            -0.500,  0.866, 0.000, -0.500,  0.866, 0.000, 0.333, 0.000,
            -0.860,  0.500, 0.000, -0.860,  0.500, 0.000, 0.417, 0.000,
            -1.000,  0.000, 0.000, -1.000,  0.000, 0.000, 0.500, 0.000,
            -0.866, -0.500, 0.000, -0.866, -0.500, 0.000, 0.583, 0.000,
            -0.500, -0.866, 0.000, -0.500, -0.866, 0.000, 0.667, 0.000,
            0.000, -1.000, 0.000,  0.000, -1.000, 0.000, 0.750, 0.000,
            0.500, -0.866, 0.000,  0.500, -0.866, 0.000, 0.833, 0.000,
            0.866, -0.500, 0.000,  0.866, -0.500, 0.000, 0.917, 0.000,
            1.000,  0.000, 0.000,  1.000,  0.000, 0.000, 1.000, 0.000,
            1.000,  0.000, 1.000,  1.000,  0.000, 0.000, 0.000, 1.000,
            0.866,  0.500, 1.000,  0.866,  0.500, 0.000, 0.083, 1.000,
            0.500,  0.866, 1.000,  0.500,  0.866, 0.000, 0.167, 1.000,
            0.000,  1.000, 1.000,  0.000,  1.000, 0.000, 0.250, 1.000,
            -0.500,  0.866, 1.000, -0.500,  0.866, 0.000, 0.333, 1.000,
            -0.860,  0.500, 1.000, -0.860,  0.500, 0.000, 0.417, 1.000,
            -1.000,  0.000, 1.000, -1.000,  0.000, 0.000, 0.500, 1.000,
            -0.866, -0.500, 1.000, -0.866, -0.500, 0.000, 0.583, 1.000,
            -0.500, -0.866, 1.000, -0.500, -0.866, 0.000, 0.667, 1.000,
            0.000, -1.000, 1.000,  0.000, -1.000, 0.000, 0.750, 1.000,
            0.500, -0.866, 1.000,  0.500, -0.866, 0.000, 0.833, 1.000,
            0.866, -0.500, 1.000,  0.866, -0.500, 0.000, 0.917, 1.000,
            1.000,  0.000, 1.000,  1.000,  0.000, 0.000, 1.000, 1.000
    };
    unsigned int indices[72] = {
            0,  1, 13, 1,  2, 14, 2,  3, 15,  3,  4, 16,  4,  5, 17,  5,  6, 18,
            6,  7, 19, 7,  8, 20, 8,  9, 21,  9, 10, 22, 10, 11, 23, 11, 12, 24,
            1, 14, 13, 2, 15, 14, 3, 16, 15,  4, 17, 16,  5, 18, 17,  6, 19, 18,
            7, 20, 19, 8, 21, 20, 9, 22, 21, 10, 23, 22, 11, 24, 23, 12, 25, 24
    };
};

static float getPixelHeight(unsigned char* data, int width, int x, int y,int hScale)
{
    float height = 0;
    if (x < 0 || x >= width || y < 0 || y >= width)
        height = 0.0f;
    else
    {
        float value = static_cast<float>(*(data + (x*width + y)));
        height = hScale * (value / 255.0f);
    }
    return height;
}

static Mesh makePlane(float width,float height,int wSeg,int hSeg)
{
    float halfWidth = width/2;
    float halfHeight = height/2;

    int gridX = std::floor(wSeg);
    int gridY = std::floor(hSeg);

    int gridX1 = gridX + 1;
    int gridY1 = gridY + 1;

    float segWidth = width / gridX;
    float segHeight = height / gridY;
    int w;
    int h;
    int comp;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* image = stbi_load("../Textures/hMap2.png", &w, &h, &comp, 1);
    stbi_set_flip_vertically_on_load(true);
    if(image == nullptr)
        throw(std::string("Failed to load heightmap"));
    std::cout<<w<<std::endl;
    std::vector<Vertex> vertex;
    std::vector<Texture> text;
    std::vector<unsigned int> indices;
    srand(time(NULL));
    float hMap = 10;
    //Vertices and texcoords
    for (int iy = 0; iy < gridY1; iy++)
    {
        float z = iy * segHeight - halfHeight;
        for (int ix = 0; ix < gridX1; ix++)
        {
            Vertex auxVert;
            float x = ix * segWidth - halfWidth;
            int index = (int)w * iy + ix;
            float y = getPixelHeight(image,w,iy,ix,hMap);
            auxVert.Position = glm::vec3(x,-15+y,z);
            auxVert.TexCoords = glm::vec2(((float)ix/(gridX)),(float)iy/(gridY))*glm::vec2(wSeg,hSeg) ;
            vertex.push_back(auxVert);
        }
    }
    //Indices
    for (int iy = 0; iy < gridY; iy++)
    {
        for (int ix=0; ix < gridX; ix++)
        {
            uint a = ix+gridX1*iy;
            uint b = ix + gridX1 * (iy + 1);
            uint c = (ix + 1) + gridX1 * (iy + 1);
            uint d = (ix + 1) + gridX1 * iy;
            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(d);
            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }
    //Normals
    for (int i = 0; i < indices.size();i+=3)
    {
        unsigned int iA = indices[i];
        unsigned int iB = indices[i+1];
        unsigned int iC = indices[i+2];
        auto edgeAB = vertex[iB].Position - vertex[iA].Position;
        auto edgeAC = vertex[iC].Position - vertex[iA].Position;
        auto norm = glm::normalize(glm::cross(edgeAB,edgeAC));
        vertex[iA].Normal += norm;
        vertex[iB].Normal += norm;
        vertex[iC].Normal += norm;
    }
    for (int i = 0; i < vertex.size();i++)
    {
        vertex[i].Normal = glm::normalize(vertex[i].Normal);
    }
    Mesh terrain(vertex,indices,"grassy2.png","../Textures/");
    return terrain;
}

static Mesh procesMesh(float vertices[], unsigned int indices[],const char * path, std::string dir, int nVertices, int nIndices)
{
    std::vector<Vertex> vertexes;
    std::vector<Texture> text;
    std::vector<unsigned int > indexes(indices,indices+nIndices);
    for (int i = 0; i < nVertices; i+=8)
    {
        Vertex actualVertex;
        actualVertex.Position = glm::vec3(vertices[i],vertices[i+1],vertices[i+2]);
        actualVertex.Normal = glm::vec3(vertices[i+3],vertices[i+4],vertices[i+5]);
        actualVertex.TexCoords = glm::vec2(vertices[i+6],vertices[i+7]);
        vertexes.push_back(actualVertex);
    }
    Mesh res(vertexes, indexes, path, dir);
    return res;
}

#endif //OPENGLTEST_BASICSHAPEBUILDER_H
