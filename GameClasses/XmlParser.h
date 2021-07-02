//
// Created by drive on 2/7/21.
//

#ifndef OPENGLTEST_XMLPARSER_H
#define OPENGLTEST_XMLPARSER_H
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "rapidxml/rapidxml.hpp"

using namespace rapidxml;
using namespace std;
class XmlParser {
public:
    XmlParser(std::string path);
    void parseDoc();
private:
    std::string path;
};


#endif //OPENGLTEST_XMLPARSER_H
