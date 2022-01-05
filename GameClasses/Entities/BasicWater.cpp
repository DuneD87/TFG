//
// Created by drive on 2/8/21.
//

#include "BasicWater.h"
#include "../Util/BasicShapeBuilder.h"

BasicWater::BasicWater(float width, float height, float wSeg, float hSeg, int waterLevel)
:width(width),height(height),wSeg(wSeg),hSeg(hSeg),waterLevel(waterLevel){
    type = 4;
    id = 138;
    setupWater();
}



void BasicWater::setupWater() {
    std::pair<std::vector<Vertex>,std::vector<unsigned int>> *plane = makePlane(width,height,wSeg,hSeg,0);
    std::vector<Texture> text;
    Texture t;
    t.type = "material";
    t.ka = glm::vec3(1.0f);
    t.kd = glm::vec3(0.5f);
    t.ks = glm::vec3(0.5f);
    text.push_back(t);
    waterMesh = new Mesh(plane->first,plane->second,"reflection.jpg","");
    waterMesh->position = glm::vec3(0,waterLevel,0);
}

void BasicWater::draw(Shader &shader, bool outlined, int depthMap) {
    waterMesh->Draw(shader,false,-1,false,false);
}

std::string BasicWater::toString() {
    return std::string();
}

void BasicWater::draw() {
    waterMesh->Draw(entityShader,false,-1,false,false);
}

