//
// Created by drive on 2/8/21.
//

#ifndef OPENGLTEST_PLANET_H
#define OPENGLTEST_PLANET_H

#include "../Entity.h"
#include "Icosphere.h"
#include "../BasicTerrain.h"
#include "Atmosphere.h"

class Planet : public Entity {
public:
    Planet(float radius, int nSeg, glm::vec3 position, Camera *cam);
    ~Planet();
    void draw(Shader &shader, bool outlined = false, int depthMap = -1);
    void renderGui();
private:
    void setupMesh();


    const char * noiseTypes[6] = {"Value","OpenSimplex2" ,"Cellular", "OpenSimplex2S", "Perlin", "ValueCubic"};
    const char * fractalTypes[6] = {"None","DomainWarpProg","FBm" ,"DomainWarpInd", "PingPong", "Ridged"};
    const char * cellDistFunc[4] = {"Hybrid","Euclidean","Euclidean Squared","Manhattan"};
    const char * cellReturnType[7] = {"CellValue","Distance2Div","Distance","Distance2","Distance2Add","Distance2Sub","Distance2Mul"};
    const char * domainWarpType[3] = {"BasicGrid","Open Simplex2", "Open Simplex2 Reduced"};
    const FastNoiseLite::NoiseType  _noiseTypes[7] = {FastNoiseLite::NoiseType_Value,
                                                    FastNoiseLite::NoiseType_OpenSimplex2,
                                                    FastNoiseLite::NoiseType_Cellular,
                                                    FastNoiseLite::NoiseType_OpenSimplex2S,
                                                    FastNoiseLite::NoiseType_Perlin,
                                                    FastNoiseLite::NoiseType_ValueCubic};
    const FastNoiseLite::FractalType _fractalTypes[6] = {FastNoiseLite::FractalType_None,
                                                         FastNoiseLite::FractalType_DomainWarpProgressive,
                                                         FastNoiseLite::FractalType_FBm,
                                                         FastNoiseLite::FractalType_DomainWarpIndependent,
                                                         FastNoiseLite::FractalType_PingPong,
                                                         FastNoiseLite::FractalType_Ridged};
    const FastNoiseLite::CellularDistanceFunction _cellDistFunc[4] = {FastNoiseLite::CellularDistanceFunction_Hybrid,
                                                                      FastNoiseLite::CellularDistanceFunction_Euclidean,
                                                                      FastNoiseLite::CellularDistanceFunction_EuclideanSq,
                                                                      FastNoiseLite::CellularDistanceFunction_Manhattan};
    const FastNoiseLite::CellularReturnType _cellReturnType[7] = {FastNoiseLite::CellularReturnType_CellValue,
                                                                  FastNoiseLite::CellularReturnType_Distance2Div,
                                                                  FastNoiseLite::CellularReturnType_Distance,
                                                                  FastNoiseLite::CellularReturnType_Distance2,
                                                                  FastNoiseLite::CellularReturnType_Distance2Add,
                                                                  FastNoiseLite::CellularReturnType_Distance2Sub,
                                                                  FastNoiseLite::CellularReturnType_Distance2Mul};
    const FastNoiseLite::DomainWarpType _domWarpType[4] = {FastNoiseLite::DomainWarpType_BasicGrid,
                                                           FastNoiseLite::DomainWarpType_OpenSimplex2,
                                                           FastNoiseLite::DomainWarpType_OpenSimplex2Reduced};
    std::vector<BasicTerrain*> faces;
public:
    float getHighestPoint() const;

    float getLowestPoint() const;

private:
    float highestPoint, lowestPoint,radius;
    float hPointOffset = 0;
    float lPointOffset = 17.452;
    float maxHeight = 800;
public:
    float getRadius() const;

    float getHPointOffset() const;

    float getLPointOffset() const;

    float getMaxHeight() const;

    int getNSeg() const;

private:
    int seed = 1337;
    float noiseFreq = 0.0005;
    int octaves = 8;
    float lacunarity = 2.3;
    float fGain = 0.5;
    float fWeStr = 0.6;
    float fPinPonStr = 3;
    float cellJitter = 1.0;
    float domWarpAmp = 50;
    float minValue = 0;
    int noiseTypeSel = 4, fractalTypeSel = 2,cellDistTypeSel = 3, cellReturnTypeSel = 2, domWarpTypeSel = 2;
    int nSeg;
    Mesh* planet = NULL;
    Atmosphere * skyDome;
    Camera * cam;
};


#endif //OPENGLTEST_PLANET_H
