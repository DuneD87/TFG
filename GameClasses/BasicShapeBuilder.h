//
// Created by drive on 4/7/21.
//

#ifndef OPENGLTEST_BASICSHAPEBUILDER_H
#define OPENGLTEST_BASICSHAPEBUILDER_H
#include <iostream>
#include <vector>
#include "Mesh.h"
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
static Mesh procesMesh(float vertices[], unsigned int indices[], int nVertices, int nIndices)
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
    Mesh res(vertexes,indexes,text);
    return res;
}
#endif //OPENGLTEST_BASICSHAPEBUILDER_H
