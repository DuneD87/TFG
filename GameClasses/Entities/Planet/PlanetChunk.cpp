//
// Created by drive on 3/8/21.
//

#include "PlanetChunk.h"

PlanetChunk::PlanetChunk(int resolution, glm::vec3 localUp,glm::vec3 position ,std::vector<Texture> textures) {
    this->resolution = resolution;
    this->position = position;
    this->textures = textures;
}

void PlanetChunk::setupMesh() {

}
