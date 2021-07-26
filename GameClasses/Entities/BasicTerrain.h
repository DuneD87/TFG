//
// Created by drive on 21/7/21.
//

#ifndef OPENGLTEST_BASICTERRAIN_H
#define OPENGLTEST_BASICTERRAIN_H

#include "Entity.h"
#include "../BasicShapeBuilder.h"
class BasicTerrain : public Entity{
public:
    virtual ~BasicTerrain();

    BasicTerrain(float width, float height, int wSeg, int hSeg,const glm::vec3 &position,
                 const std::vector<Texture> &textures, const char * path);
    void draw(Shader &shader, bool outlined, int depthMap);
private:
    float width,height;
    int wSeg,hSeg;
    glm::vec3 position;
    std::vector<Texture> textures;
    Mesh *terrainMesh;
    int vertexCount;
    void setupMesh(const char * path);
    float lerp(float x, float y, float t) {return x + (y - x) * t;}
    float getPixelHeight(unsigned char* data, float x, float y,float w,float hScale);
    float bilinearSample(float x,float,unsigned char* data, float width);
};


#endif //OPENGLTEST_BASICTERRAIN_H
