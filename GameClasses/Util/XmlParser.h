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
#include "BasicShapeBuilder.h"
using namespace rapidxml;
using namespace std;
class XmlParser {
public:

    vector<Entity*> _ents;

    int nPointLights;
    int entIndex;
    XmlParser(std::string path,Camera *cam);

private:
    xml_node<> * _rootNode;
    std::string path;
    Camera * cam;
    int nPlanets;
    Light* getLight(xml_node<> *light);
    PhysicsObject* getObject(xml_node<> *model);
    Planet* getPlanet(xml_node<> *planet);
    vector<Mesh*> getSprites();

    glm::vec3 getValues3(xml_node<> *pos);
    glm::vec4 getValues4(xml_node<> *rot);
};


#endif //OPENGLTEST_XMLPARSER_H
