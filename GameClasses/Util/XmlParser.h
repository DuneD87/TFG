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
    vector<Shader> shaders;
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
};


#endif //OPENGLTEST_XMLPARSER_H
