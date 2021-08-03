//
// Created by drive on 2/8/21.
//

#include "Planet.h"
#include "Cubesphere.h"
#include "../../Util/FastNoiseLite.h"

Planet::Planet(float radius, int nSeg, bool smoot) {
    this->type = 3;
    this->id = 34324;
    this->radius = radius;
    _position = glm::vec3(0);
    const char *textures[] = {"grassy2.png","grassFlowers.png","mud.png","seamless_rock2.png"};
    //up,down,left,right,front,back
    auto *cubeSphere = new Cubesphere(radius,nSeg,true);
    std::cout<<"Vertex count: "<<cubeSphere->vertexList.size()<<std::endl;
    std::vector<unsigned int> indices;
    const unsigned int * cubeIndex = cubeSphere->getIndices();
    for (int i = 0; i < cubeSphere->getIndexCount();i++)
        indices.push_back(cubeIndex[i]);
    highestPoint = cubeSphere->getHPoint();
    lowestPoint = cubeSphere->getLPoint();
    std::cout<<"hPoint: "<<highestPoint+radius<<" lPoint: "<<lowestPoint-radius<<std::endl;
    planet = new Mesh(cubeSphere->vertexList,indices,textures,4);
}




void Planet::draw(Shader &shader, bool outlined, int depthMap) {

    shader.setInt("isTerrain",1);
    shader.setFloat("hPoint",highestPoint);
    shader.setFloat("lPoint",lowestPoint);
    shader.setFloat("pRadius",radius);
    shader.setVec3("upVector",_position);
    planet->Draw(shader,outlined,depthMap);
    shader.setInt("isTerrain",0);

}

Planet::~Planet() {
    for (auto face: faces)
        delete face;
}

