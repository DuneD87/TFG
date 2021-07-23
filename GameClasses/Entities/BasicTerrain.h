//
// Created by drive on 21/7/21.
//

#ifndef OPENGLTEST_BASICTERRAIN_H
#define OPENGLTEST_BASICTERRAIN_H

#include "Entity.h"
#include "../BasicShapeBuilder.h"
class BasicTerrain : public Entity{
public:
    BasicTerrain(float width, float height, int wSeg, int hSeg,const glm::vec3 &position, const std::vector<Texture> &textures);
    void draw(Shader &shader, bool outlined, int depthMap);
private:
    float width,height;
    int wSeg,hSeg;
    glm::vec3 position;
    std::vector<Texture> textures;
    Mesh terrainMesh;
    int vertexCount;
    void setupMesh();
};


#endif //OPENGLTEST_BASICTERRAIN_H
