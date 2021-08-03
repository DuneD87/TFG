//
// Created by drive on 3/8/21.
//

#include "TerrainChunk.h"

TerrainChunk::TerrainChunk(int resolution, glm::vec3 localUp, glm::vec3 position, const char **path) {
    this->resolution = resolution;
    this->localY = localUp;
    this->position = position;
    axisX = glm::vec3(localUp.y,localUp.x,localUp.z);
    axisZ = glm::cross(localUp,axisX);
}

void TerrainChunk::setupMesh() {

}
