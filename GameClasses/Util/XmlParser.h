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
    XmlParser(std::string path);

private:
    xml_node<> * _rootNode;
    std::string path;

    Light* getLight(xml_node<> *light);
    PhysicsObject* getObject(xml_node<> *model);
    vector<Mesh*> getSprites();

    glm::vec3 getValues3(xml_node<> *pos);
    glm::vec4 getValues4(xml_node<> *rot);
};


#endif //OPENGLTEST_XMLPARSER_H
