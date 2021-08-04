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

    void renderGui();

    const char * noiseTypes[7] = {"None","Value","OpenSimplex2" ,"Cellular", "OpenSimplex2S", "Perlin", "ValueCubic"};
    const char * fractalTypes[6] = {"None","DomainWarpProg","FBm" ,"DomainWarpInd", "PingPong", "Ridged"};
    const char * cellDistFunc[5] = {"None","Hybrid","Euclidean","Euclidean Squared","Manhattan"};
    const char * cellReturnType[8] = {"None","CellValue","Distance2Div","Distance","Distance2","Distance2Add","Distance2Sub","Distance2Mul"};
    const char * domainWarpType[4] = {"None","BasicGrid","Open Simplex2", "Open Simplex2 Reduced"};
    std::vector<BasicTerrain*> faces;
    float highestPoint, lowestPoint,radius;
    float hPointOffset = 0;
    float lPointOffset = 0;
    float seed = 0;
    float noiseFreq = 0;
    float octaves = 0;
    float lacunarity = 0;
    float fGain = 0;
    float fWeStr = 0;
    float fPinPonStr = 0;
    float cellJitter = 0;
    float domWarpAmp = 0;

    Mesh* planet;
};


#endif //OPENGLTEST_PLANET_H
