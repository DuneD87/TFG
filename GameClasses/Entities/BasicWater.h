//
// Created by drive on 2/8/21.
//

#ifndef OPENGLTEST_BASICWATER_H
#define OPENGLTEST_BASICWATER_H

#include "Entity.h"
#include "../Util/FastNoiseLite.h"

class BasicWater : public Entity{
public:
    BasicWater(float width,float height,float wSeg, float hSeg,int waterLevel);
    void draw(Shader &shader, bool outlined = false, int depthMap = -1);
    std::string toString();
private:

    void setupWater();
    int waterLevel;
    float width,height,wSeg,hSeg;
    Mesh *waterMesh;
};


#endif //OPENGLTEST_BASICWATER_H
