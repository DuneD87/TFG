//
// Created by drive on 2/8/21.
//

#ifndef OPENGLTEST_PLANET_H
#define OPENGLTEST_PLANET_H

#include "../Entity.h"
#include "Atmosphere.h"
#include "WaterSphere.h"
#include "../../Base/EngineSettings.h"
#include "../Light.h"

class Planet : public Entity {
public:

    Planet(float radius, int nSeg, bool hasAtmos, float maxHeight,const std::vector<std::string> &path,glm::vec3 position,
           bool hasWater, FastNoiseLite Noise, int seed);

    ~Planet();
    void draw(Shader &shader, bool outlined = false, int depthMap = -1);
    void renderGui();
    void addCamera(Camera * cam);
    void addBiome(Biome bio);
    void setupEffects(float atmosRadius, float kr, float km, float e, float h, float l,
                      float gm, float numOutScatter, float numInScatter, float scale,
                      glm::vec3 color);
    void setSun(Light* sun);
    void setupNoiseVariables(float noiseFreq, int octaves, float lacunarity,
                             float fGain, float fWeStr, float fPinPonStr, float cellJitter, float domWarpAmp, float minValue,
                             int noiseTypeSel, int fractalTypeSel, int cellDistTypeSel, int cellReturnTypeSel, int domWarpTypeSel);
    float getHighestPoint() const;
    float getLowestPoint() const;
    float getRadius() const;
    void setNoDrawEffects(bool drawEffects);
    void setWaterTexture(unsigned int idText);
    bool hasAtmosphere() const;
    std::string getAtmosSettings() const;
    std::string toString();
private:
    void setupMesh();
    void bindPlanetTextures();

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

    float hPointOffset = 0;
    FastNoiseLite noise;
    float lPointOffset = 17.452;
    int seed;
    bool biomeSet = false;
    float radius;
    int noiseTypeSel,fractalTypeSel,cellDistTypeSel, cellReturnTypeSel, domWarpTypeSel,nSeg, octaves;
    bool hasAtmos, hasWater, drawEffects;
    float maxHeight, noiseFreq, lacunarity, fGain, fWeStr, fPinPonStr,
     cellJitter, domWarpAmp, minValue,highestPoint, lowestPoint;
    float blendFactor = 0.5;
    float depthBlend = 0.5;
    unsigned int noiseMap;
    std::vector<std::string> path, pathNormal;
    Mesh* planet = NULL;
    Atmosphere * skyDome;
    Camera * cam;
    std::vector<Texture> textures;
    std::vector<Biome> biomes;
    WaterSphere *water;
};


#endif //OPENGLTEST_PLANET_H
