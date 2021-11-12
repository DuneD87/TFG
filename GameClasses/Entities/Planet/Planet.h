//
// Created by drive on 2/8/21.
//

#ifndef OPENGLTEST_PLANET_H
#define OPENGLTEST_PLANET_H

#include "../Entity.h"
#include "../../../OldStuff/BasicTerrain.h"
#include "Atmosphere.h"
#include "WaterSphere.h"
#include "../../Base/EngineSettings.h"
class Planet : public Entity {
public:

    Planet(float radius, int nSeg, glm::vec3 position, Camera *cam, std::vector<std::string> path);
    Planet(float radius, int nSeg, bool hasAtmos, float maxHeight, float noiseFreq, int octaves, float lacunarity,
           float fGain, float fWeStr, float fPinPonStr, float cellJitter, float domWarpAmp, float minValue,
           int noiseTypeSel, int fractalTypeSel, int cellDistTypeSel, int cellReturnTypeSel, int domWarpTypeSel,
           const std::vector<std::string> &path, const std::vector<std::string> &pathNorm, glm::vec3 position,bool hasWater);

    ~Planet();
    void draw(Shader &shader, bool outlined = false, int depthMap = -1);
    void renderGui();
    void addCamera(Camera * cam);
    void addBiome(Biome bio);
    void setupUniformBuffers();
    void setupAtmosphere(float atmosRadius, float kr, float km, float e, float h, float l,
                         float gm, float numOutScatter, float numInScatter, float scale,
                         glm::vec3 color);
    void setSunDir(glm::vec3 sunDir);
    float getHighestPoint() const;
    float getLowestPoint() const;
    float getRadius() const;
    float getHPointOffset() const;
    float getLPointOffset() const;
    float getMaxHeight() const;
    int getNSeg() const;
    bool hasAtmosphere() const;
    std::string getAtmosSettings() const;
    std::string toString();
private:
    void setupMesh();
    void bindPlanetTextures();
    void addVegetation();
    std::vector<Entity*> planetDeco;
    const char *decoPaths[14] = {
            "../Models/treeLP1.obj",
            "../Models/treeLP2.obj",
            "../Models/grass1.obj",
            "../Models/grass2.obj",
            "../Models/rock1.obj",
            "../Models/rock2.obj",
            "../Models/rock3.obj",
            "../Models/rock4.obj",
            "../Models/bark1.obj",
            "../Models/bark2.obj",
            "../Models/shroom1.obj",
            "../Models/shroom2.obj",
            "../Models/bush1.obj",
            "../Models/bush2.obj"
    };
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

    float highestPoint, lowestPoint;
    float hPointOffset = 0;
    float lPointOffset = 17.452;
    int seed = 1337;

    float radius;
    int nSeg;
    bool hasAtmos, hasWater;
    float maxHeight;
    float noiseFreq;

    int octaves;
    float lacunarity;
    float fGain;
    float fWeStr;
    float fPinPonStr;
    float cellJitter;
    float domWarpAmp;
    float minValue;
    int noiseTypeSel,fractalTypeSel,cellDistTypeSel, cellReturnTypeSel, domWarpTypeSel;
    bool biomeSet = false;
    float blendFactor = 0.5;
    float depthBlend = 0.5;
    unsigned int noiseMap;
    std::vector<std::string> path;
    std::vector<std::string> pathNormal;
    Mesh* planet = NULL;
    Atmosphere * skyDome;
    Camera * cam;
    std::vector<Texture> textures;
    std::vector<Biome> biomes;
    WaterSphere *water;
};


#endif //OPENGLTEST_PLANET_H
