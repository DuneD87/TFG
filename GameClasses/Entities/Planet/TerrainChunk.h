//
// Created by drive on 3/8/21.
//

#ifndef OPENGLTEST_TERRAINCHUNK_H
#define OPENGLTEST_TERRAINCHUNK_H

#include "../Entity.h"

class TerrainChunk {
public:
    TerrainChunk(int resolution, glm::vec3 localUp,glm::vec3 position ,const char ** path);
private:
    void setupMesh();
    Mesh *terrainChunk;
    int resolution;
    glm::vec3 localY,axisX,axisZ,position;
};


#endif //OPENGLTEST_TERRAINCHUNK_H
