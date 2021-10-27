//
// Created by drive on 21/7/21.
//

#ifndef OPENGLTEST_BASICTERRAIN_H
#define OPENGLTEST_BASICTERRAIN_H

#include "../GameClasses/Entities/Entity.h"
#include "../GameClasses/Util/Math.h"
#include "../GameClasses/Util/FastNoiseLite.h"
#include "../GameClasses/Util/BasicShapeBuilder.h"
class BasicTerrain : public Entity{
public:
    virtual ~BasicTerrain();
    Mesh *terrainMesh;
    BasicTerrain(float width, float height, int wSeg, int hSeg,const glm::vec3 &position,
                 const char ** path, uint nTextures, glm::vec3 up = glm::vec3(0,1,0));
    void translateTerrain(glm::vec3 position) {terrainMesh->position = position;}
    void rotateTerrain(glm::vec4 rotate) {terrainMesh->angle = rotate.w, terrainMesh->axis = glm::vec3(rotate.x,rotate.y,rotate.z);}
    void draw(Shader &shader, bool outlined, int depthMap);
    std::string toString();
private:
public:
    float getLowestPoint() const;

    float getHighestPoint() const;

private:
    float width,height,lowestPoint,highestPoint;
    int wSeg,hSeg;
    uint nText;
public:
    float getWidth() const;

    float getHeight() const;

    int getWSeg() const;

    int getHSeg() const;

private:
    glm::vec3 position;
    glm::vec3 upVector;
    glm::vec3 xAxis;
    glm::vec3 yAxis;
public:
    const glm::vec3 &getXAxis() const;

    const glm::vec3 &getYAxis() const;

private:
    std::vector<Texture> textures;

    void setupMesh(const char ** path);
    float getPixelHeight(unsigned char* data, float x, float y,float w,float hScale);

};


#endif //OPENGLTEST_BASICTERRAIN_H
