//
// Created by drive on 2/7/21.
//

#include "XmlParser.h"

XmlParser::XmlParser(std::string path) {
    this->path = path;
    xml_document<> doc;
    // Read the xml file into a vector
    ifstream theFile (this->path);
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    // Parse the buffer using the xml file parsing library into doc
    doc.parse<0>(&buffer[0]);
    // Find our root node
    _rootNode = doc.first_node("Scene");
    _lights = getLights();
    _models = getModels();
    _effects = getSprites();
}

vector<Light> XmlParser::getLights() {
    vector<Light> res;
    xml_node<> * lights = _rootNode->first_node("Lights");
    //Scene lights parsing
    for (xml_node<> * light = lights->first_node("Light");light;light = light->next_sibling() )
    {
        string typePre = light->first_attribute("type")->value();

        float constant = strtof(light->first_attribute("constant")->value(),NULL);;
        float linear = strtof(light->first_attribute("linear")->value(),NULL);;
        float quadratic = strtof(light->first_attribute("quadratic")->value(),NULL);;

        xml_node<> * pos = light->first_node("Position");
        xml_node<> * amb = light->first_node("Ambient");
        xml_node<> * diff = light->first_node("Diffuse");
        xml_node<> * spec = light->first_node("Specular");
        glm::vec3 direction;//To implement

        glm::vec3 position = getValues3(pos);
        glm::vec3 ambient = getValues3(amb);
        glm::vec3 diffuse = getValues3(diff);
        glm::vec3 specular = getValues3(spec);
        Light lightAux(typePre,ambient,diffuse,specular,constant,linear,quadratic);
        if (typePre == "pointLight")
            lightAux.setPosition(position);
        else
            lightAux.setDirection(direction);
        res.push_back(lightAux);
    }

    return res;
}

vector<Model> XmlParser::getModels() {
    std::vector<Model> res;
    xml_node<> * models = _rootNode->first_node("Models");

    //Scene model parsing
    for (xml_node<> * model = models->first_node("Model");model;model = model->next_sibling() )
    {
        std::string typePre = model->first_attribute("type")->value();

        xml_node<> * pos = model->first_node("Position");
        xml_node<> * rot = model->first_node("Rotation");
        xml_node<> * scale = model->first_node("Scale");

        Model modelRes;
        if (typePre == "Cube") {
            const char * path = (const char * )model->first_attribute("path")->value();
            std::string directory = model->first_attribute("directory")->value();
            Cube cube;
            Mesh cubeMesh = procesMesh(cube.vertices, cube.indices,path,directory,192,36);
            modelRes.meshes.push_back(cubeMesh);

        }
        else if (typePre == "AModel")
        {
            const char * path = (const char * )model->first_attribute("path")->value();
            modelRes = Model(path);
        }
        modelRes.setPosition(getValues3(pos));
        glm::vec4 rotation = getValues4(rot);
        modelRes.setRotation(rotation.w, glm::vec3(rotation));
        modelRes.setScale(getValues3(scale));

        res.push_back(modelRes);
    }

    return res;
}

vector<Mesh> XmlParser::getSprites() {
    std::vector<Mesh> res;
    xml_node<> * models = _rootNode->first_node("Effects");

    //Scene model parsing
    int i = 0;
    for (xml_node<> * effect = models->first_node("Effect"); effect; effect = effect->next_sibling() )
    {
        std::string typePre = effect->first_attribute("type")->value();

        xml_node<> * pos = effect->first_node("Position");
        xml_node<> * rot = effect->first_node("Rotation");
        xml_node<> * scale = effect->first_node("Scale");

        Mesh cubeMesh;
        if (typePre == "Sprite" || typePre == "Window") {
            const char * path = (const char * )effect->first_attribute("path")->value();
            std::string directory = effect->first_attribute("directory")->value();
            Quad quad;
            cubeMesh = procesMesh(quad.vertices, quad.indices,path,directory,32,6);
        }

        cubeMesh.position = getValues3(pos);
        glm::vec4 rotation = getValues4(rot);
        cubeMesh.axis= glm::vec3(rotation);
        cubeMesh.angle = rotation.w;
        cubeMesh.scale = getValues3(scale);

        res.push_back(cubeMesh);
    }
    return res;
}

glm::vec3 XmlParser::getValues3(xml_node<> *pos) {
    float xPos = strtof(pos->first_attribute("x")->value(),NULL);
    float yPos = strtof(pos->first_attribute("y")->value(),NULL);
    float zPos = strtof(pos->first_attribute("z")->value(),NULL);
    return glm::vec3(xPos,yPos,zPos);
}

glm::vec4 XmlParser::getValues4(xml_node<> *rot) {
    float xRot = strtof(rot->first_attribute("x")->value(),NULL);
    float yRot = strtof(rot->first_attribute("y")->value(),NULL);
    float zRot = strtof(rot->first_attribute("z")->value(),NULL);
    float angle = strtof(rot->first_attribute("angle")->value(),NULL);
    return glm::vec4(xRot,yRot,zRot,angle);
}


