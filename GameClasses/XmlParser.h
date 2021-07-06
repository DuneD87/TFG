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
#include "Light.h"
#include "Model.h"
#include "rapidxml/rapidxml.hpp"

using namespace rapidxml;
using namespace std;
class XmlParser {
public:
    XmlParser(std::string path);
    vector<Light> getLights();
    vector<Model> getModels();
private:
    xml_node<> * getRootNode();
    std::string path;
    vector<Light> _lights;
};


#endif //OPENGLTEST_XMLPARSER_H
