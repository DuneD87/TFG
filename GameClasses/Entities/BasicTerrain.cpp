//
// Created by drive on 21/7/21.
//

#include "BasicTerrain.h"

BasicTerrain::BasicTerrain(float width, float height, int wSeg, int hSeg, const glm::vec3 &position,
                           const std::vector<Texture> &textures):width(width),height(height),wSeg(wSeg),hSeg(hSeg),
                           position(position),textures(textures){
    this->type = 3;
    setupMesh();
}



void BasicTerrain::setupMesh() {
    float vertices[256*256*8];
    unsigned int indices[(256*256) + (255)*(254)];

    terrainMesh = makePlane(width,height,wSeg,hSeg);
}

void BasicTerrain::draw(Shader &shader, bool outLined, int depthMap) {
    terrainMesh.Draw(shader,outLined,depthMap);
}

