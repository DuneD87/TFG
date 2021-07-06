//
// Created by drive on 2/7/21.
//

#include "XmlParser.h"

XmlParser::XmlParser(std::string path) {
    this->path = path;
}

vector<Light> XmlParser::getLights() {
    xml_document<> doc;
    xml_node<> * root_node = getRootNode();

    for (xml_node<> * lights = root_node->first_node("Lights"); lights; lights = lights->next_sibling())
    {
        //Scene lights parsing
        for (xml_node<> * light = lights->first_node("Light");light;light = light->next_sibling() )
        {

            string typePre = light->first_attribute("type")->value();
            Type type;
            if (typePre == "pointLight")
                type = PointLight;
            else
                type = DirLight;
            float constant = strtod(light->first_attribute("constant")->value(),NULL);;
            float linear = strtod(light->first_attribute("linear")->value(),NULL);;
            float quadratic = strtod(light->first_attribute("quadratic")->value(),NULL);;
            printf("Light found with type %s and:\n\tConstant: %f\n\tLinear: %f\n\tQuadratic: %f\n" ,
                   typePre.c_str(),
                   constant,
                   linear,
                   quadratic
            );

            xml_node<> * pos = light->first_node("Position");
            float x = strtof(pos->first_attribute("x")->value(),NULL);
            float y = strtof(pos->first_attribute("y")->value(),NULL);
            float z = strtof(pos->first_attribute("z")->value(),NULL);
            glm::vec3 position(x,y,z);
            printf("\tPosition:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n",x,y,z);
            xml_node<> * amb = light->first_node("Ambient");
            x = strtof(amb->first_attribute("x")->value(),NULL);
            y = strtof(amb->first_attribute("y")->value(),NULL);
            z = strtof(amb->first_attribute("z")->value(),NULL);
            printf("\tAmbient:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n",x,y,z);
            glm::vec3 ambient(x,y,z);
            xml_node<> * diff = light->first_node("Diffuse");
            x = strtof(diff->first_attribute("x")->value(),NULL);
            y = strtof(diff->first_attribute("y")->value(),NULL);
            z = strtof(diff->first_attribute("z")->value(),NULL);
            printf("\tDiffuse:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n",x,y,z);
            glm::vec3 diffuse(x,y,z);
            xml_node<> * spec = light->first_node("Specular");
            x = strtof(spec->first_attribute("x")->value(),NULL);
            y = strtof(spec->first_attribute("y")->value(),NULL);
            z = strtof(spec->first_attribute("z")->value(),NULL);
            printf("\tSpecular:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n",x,y,z);
            glm::vec3 specular(x,y,z);
            glm::vec3 direction;//To implement
            Light lightAux(type,ambient,diffuse,specular,constant,linear,quadratic);
            if (type == PointLight)
                lightAux.setPosition(position);
            else
                lightAux.setDirection(direction);
            _lights.push_back(lightAux);
        }
    }
    return _lights;
}

vector<Model> XmlParser::getModels() {
    std::vector<Model> res;
    xml_node<> * root_node = getRootNode();
    for (xml_node<> * lights = root_node->first_node("Models"); lights; lights = lights->next_sibling())
    {

    }
    return res;
}

xml_node<> *XmlParser::getRootNode() {

    xml_document<> doc;
    xml_node<> * root_node;
    // Read the xml file into a vector
    ifstream theFile (this->path);
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    // Parse the buffer using the xml file parsing library into doc
    doc.parse<0>(&buffer[0]);
    // Find our root node
    root_node = doc.first_node("Scene");
    // Iterate over the brewerys
    return root_node;
}
