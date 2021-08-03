//
// Created by drive on 2/8/21.
//

#ifndef OPENGLTEST_PLANET_H
#define OPENGLTEST_PLANET_H

#include "../Entity.h"
#include "Icosphere.h"
#include "../BasicTerrain.h"

class Planet : public Entity {
public:
    Planet(float radius, int nSeg, bool smoot);
    ~Planet();
    void draw(Shader &shader, bool outlined = false, int depthMap = -1);
private:
    std::vector<BasicTerrain*> faces;
    float highestPoint, lowestPoint,radius;
    Mesh* planet;
};


#endif //OPENGLTEST_PLANET_H
