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
    entIndex = 0;
    xml_node<> * entities = _rootNode->first_node("Entities");
    for (xml_node<> * ent = entities->first_node("Entity");ent;ent = ent->next_sibling())
    {
        string entType = ent->first_attribute("type")->value();
        if (entType == "PhysicsObject")
        {
            xml_node<> *model = ent->first_node("Model");
            _ents.push_back(getObject(model));

        } else if (entType == "Light")
        {
            xml_node<> *light = ent->first_node("Light");
            _ents.push_back(getLight(light));
        }
        entIndex++;
    }
}

Light XmlParser::getLight(xml_node<> *light) {

        string typePre = light->first_attribute("type")->value();
        xml_node<> * amb = light->first_node("Ambient");
        xml_node<> * diff = light->first_node("Diffuse");
        xml_node<> * spec = light->first_node("Specular");
        glm::vec3 ambient = getValues3(amb);
        glm::vec3 diffuse = getValues3(diff);
        glm::vec3 specular = getValues3(spec);
        if (typePre == "pointLight")
        {
            float constant = strtof(light->first_attribute("constant")->value(),NULL);;
            float linear = strtof(light->first_attribute("linear")->value(),NULL);;
            float quadratic = strtof(light->first_attribute("quadratic")->value(),NULL);;

            xml_node<> * pos = light->first_node("Position");
            glm::vec3 position = getValues3(pos);

            Light lightAux(typePre,ambient,diffuse,specular,constant,linear,quadratic,entIndex);
            lightAux.setPosition(position);
            return lightAux;
        } else if (typePre == "dirLight")
        {
            xml_node<> * dir = light->first_node("Direction");

            glm::vec3 direction = getValues3(dir);
            Light lightAux(typePre,ambient,diffuse,specular,direction,entIndex);
            lightAux.setDirection(direction);
            return lightAux;
        }
}

PhysicsObject XmlParser::getObject(xml_node<> * model) {
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

        PhysicsObject obj(entIndex,1,modelRes);
        obj.setPosition(getValues3(pos));
        glm::vec4 rotation = getValues4(rot);
        obj.setRotation(rotation);
        obj.setScale(getValues3(scale));
        return obj;
    }
    else if (typePre == "AModel")
    {
        const char * path = (const char * )model->first_attribute("path")->value();
        PhysicsObject obj(entIndex,1,path);
        obj.setPosition(getValues3(pos));
        glm::vec4 rotation = getValues4(rot);
        obj.setRotation(rotation);
        obj.setScale(getValues3(scale));
        return obj;
    }
}

vector<Mesh> XmlParser::getSprites() {
    std::vector<Mesh> res;
    xml_node<> * models = _rootNode->first_node("Effects");

    //Renderer model parsing
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


