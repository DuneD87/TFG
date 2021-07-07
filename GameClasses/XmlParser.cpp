//
// Created by drive on 2/7/21.
//

#include "XmlParser.h"

XmlParser::XmlParser(std::string path) {
    this->path = path;


}

vector<Light> XmlParser::getLights() {
    xml_document<> doc;
    // Read the xml file into a vector
    ifstream theFile (this->path);
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    // Parse the buffer using the xml file parsing library into doc
    doc.parse<0>(&buffer[0]);
    // Find our root node
    _rootNode = doc.first_node("Scene");
    xml_node<> * lights = _rootNode->first_node("Lights");

    //Scene lights parsing
    for (xml_node<> * light = lights->first_node("Light");light;light = light->next_sibling() )
    {

        string typePre = light->first_attribute("type")->value();
        Type type;
        if (typePre == "pointLight")
            type = PointLight;
        else
            type = DirLight;
        float constant = strtof(light->first_attribute("constant")->value(),NULL);;
        float linear = strtof(light->first_attribute("linear")->value(),NULL);;
        float quadratic = strtof(light->first_attribute("quadratic")->value(),NULL);;
        /*printf("Light found with type %s and:\n\tConstant: %f\n\tLinear: %f\n\tQuadratic: %f\n" ,
               typePre.c_str(),
               constant,
               linear,
               quadratic
        );*/

        xml_node<> * pos = light->first_node("Position");
        float x = strtof(pos->first_attribute("x")->value(),NULL);
        float y = strtof(pos->first_attribute("y")->value(),NULL);
        float z = strtof(pos->first_attribute("z")->value(),NULL);
        glm::vec3 position(x,y,z);
        //printf("\tPosition:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n",x,y,z);
        xml_node<> * amb = light->first_node("Ambient");
        x = strtof(amb->first_attribute("x")->value(),NULL);
        y = strtof(amb->first_attribute("y")->value(),NULL);
        z = strtof(amb->first_attribute("z")->value(),NULL);
        //printf("\tAmbient:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n",x,y,z);
        glm::vec3 ambient(x,y,z);
        xml_node<> * diff = light->first_node("Diffuse");
        x = strtof(diff->first_attribute("x")->value(),NULL);
        y = strtof(diff->first_attribute("y")->value(),NULL);
        z = strtof(diff->first_attribute("z")->value(),NULL);
        //printf("\tDiffuse:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n",x,y,z);
        glm::vec3 diffuse(x,y,z);
        xml_node<> * spec = light->first_node("Specular");
        x = strtof(spec->first_attribute("x")->value(),NULL);
        y = strtof(spec->first_attribute("y")->value(),NULL);
        z = strtof(spec->first_attribute("z")->value(),NULL);
        //printf("\tSpecular:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n",x,y,z);
        glm::vec3 specular(x,y,z);
        glm::vec3 direction;//To implement
        Light lightAux(type,ambient,diffuse,specular,constant,linear,quadratic);
        if (type == PointLight)
            lightAux.setPosition(position);
        else
            lightAux.setDirection(direction);
        _lights.push_back(lightAux);
    }

    return _lights;
}

vector<Model> XmlParser::getModels() {
    std::vector<Model> res;
    xml_document<> doc;
    // Read the xml file into a vector
    ifstream theFile (this->path);
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    // Parse the buffer using the xml file parsing library into doc
    doc.parse<0>(&buffer[0]);
    // Find our root node
    _rootNode = doc.first_node("Scene");
    xml_node<> * models = _rootNode->first_node("Models");

    //Scene model parsing
    int i = 0;
    for (xml_node<> * model = models->first_node("Model");model;model = model->next_sibling() )
    {
        std::string typePre = model->first_attribute("type")->value();

        xml_node<> * pos = model->first_node("Position");
        float xPos = strtof(pos->first_attribute("x")->value(),NULL);
        float yPos = strtof(pos->first_attribute("y")->value(),NULL);
        float zPos = strtof(pos->first_attribute("z")->value(),NULL);

        xml_node<> * rot = model->first_node("Rotation");
        float xRot = strtof(rot->first_attribute("x")->value(),NULL);
        float yRot = strtof(rot->first_attribute("y")->value(),NULL);
        float zRot = strtof(rot->first_attribute("z")->value(),NULL);
        float angle = strtof(rot->first_attribute("angle")->value(),NULL);

        xml_node<> * scale = model->first_node("Scale");
        float xSize = strtof(scale->first_attribute("x")->value(),NULL);
        float ySize = strtof(scale->first_attribute("y")->value(),NULL);
        float zSize = strtof(scale->first_attribute("z")->value(),NULL);
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
        modelRes.setPosition(glm::vec3(xPos,yPos,zPos));
        modelRes.setRotation(angle,glm::vec3(xRot,yRot,zRot));
        modelRes.setScale(glm::vec3(xSize,ySize,zSize));

        res.push_back(modelRes);
    }

    return res;
}

xml_node<> *XmlParser::getRootNode() {

}

vector<Mesh> XmlParser::getSprites() {
    std::vector<Mesh> res;
    xml_document<> doc;
    // Read the xml file into a vector
    ifstream theFile (this->path);
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    // Parse the buffer using the xml file parsing library into doc
    doc.parse<0>(&buffer[0]);
    // Find our root node
    _rootNode = doc.first_node("Scene");
    xml_node<> * models = _rootNode->first_node("Effects");

    //Scene model parsing
    int i = 0;
    for (xml_node<> * effect = models->first_node("Effect"); effect; effect = effect->next_sibling() )
    {
        std::string typePre = effect->first_attribute("type")->value();

        xml_node<> * pos = effect->first_node("Position");
        float xPos = strtof(pos->first_attribute("x")->value(),NULL);
        float yPos = strtof(pos->first_attribute("y")->value(),NULL);
        float zPos = strtof(pos->first_attribute("z")->value(),NULL);

        xml_node<> * rot = effect->first_node("Rotation");
        float xRot = strtof(rot->first_attribute("x")->value(),NULL);
        float yRot = strtof(rot->first_attribute("y")->value(),NULL);
        float zRot = strtof(rot->first_attribute("z")->value(),NULL);
        float angle = strtof(rot->first_attribute("angle")->value(),NULL);

        xml_node<> * scale = effect->first_node("Scale");
        float xSize = strtof(scale->first_attribute("x")->value(),NULL);
        float ySize = strtof(scale->first_attribute("y")->value(),NULL);
        float zSize = strtof(scale->first_attribute("z")->value(),NULL);
        Mesh cubeMesh;
        if (typePre == "Sprite") {
            const char * path = (const char * )effect->first_attribute("path")->value();
            std::string directory = effect->first_attribute("directory")->value();
            Quad quad;
            cubeMesh = procesMesh(quad.vertices, quad.indices,path,directory,32,6);
        }

        cubeMesh.position = glm::vec3(xPos,yPos,zPos);
        cubeMesh.axis= glm::vec3(xRot,yRot,zRot);
        cubeMesh.angle = angle;
        cubeMesh.scale = glm::vec3(xSize,ySize,zSize);

        res.push_back(cubeMesh);
    }



    return res;
}

