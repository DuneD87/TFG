//
// Created by drive on 2/7/21.
//

#ifndef OPENGLTEST_XMLPARSER_H
#define OPENGLTEST_XMLPARSER_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "../Entities/Light.h"
#include "../Entities/PhysicsObject.h"
#include "../Entities/Planet/Planet.h"
#include "../Basic/Model.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "BasicShapeBuilder.h"
using namespace rapidxml;
using namespace std;
class XmlParser {
public:

    vector<Entity*> _ents;
    Light *sun;
    int nPointLights;
    int entIndex;
    void saveWorld();
    XmlParser(std::string path,Camera *cam);
    void readData();
    ~XmlParser();
private:
    xml_node<> * _rootNode;
    xml_node<> * _planetRoot;
    std::vector<xml_node<> *> _planetNodes;
    xml_node<> * _sunNode;
    std::string path;
    Camera * cam;
    int nPlanets;
    Light* getLight(xml_node<> *light);
    PhysicsObject* getObject(xml_node<> *model);
    Planet* getPlanet(xml_node<> *planet);
    vector<Mesh*> getSprites();
    glm::vec3 getValues3(xml_node<> *pos);
    glm::vec4 getValues4(xml_node<> *rot);
    void setValues3(xml_node<> *origin, std::vector<string> val);
    vector<string> split(string s, string delimiter);
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
};


#endif //OPENGLTEST_XMLPARSER_H
