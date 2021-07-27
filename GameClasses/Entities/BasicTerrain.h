//
// Created by drive on 21/7/21.
//

#ifndef OPENGLTEST_BASICTERRAIN_H
#define OPENGLTEST_BASICTERRAIN_H

#include "Entity.h"
#include "../Math.h"
#include "../FastNoiseLite.h"
#include "../BasicShapeBuilder.h"
class BasicTerrain : public Entity{
public:
    virtual ~BasicTerrain();
    Mesh *terrainMesh;
    BasicTerrain(float width, float height, int wSeg, int hSeg,const glm::vec3 &position,
                 const std::vector<Texture> &textures, const char ** path, uint nTextures);
    void draw(Shader &shader, bool outlined, int depthMap);
private:
public:
    float getLowestPoint() const;

    float getHighestPoint() const;

private:
    float width,height,lowestPoint,highestPoint;
    int wSeg,hSeg;
    uint nText;
    glm::vec3 position;
    std::vector<Texture> textures;

    Mesh *tree;
    int vertexCount;
    void setupMesh(const char ** path);
    float getPixelHeight(unsigned char* data, float x, float y,float w,float hScale);
    float bilinearSample(float x,float,unsigned char* data, float width);
};


#endif //OPENGLTEST_BASICTERRAIN_H
