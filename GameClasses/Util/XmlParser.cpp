//
// Created by drive on 2/7/21.
//

#include "XmlParser.h"

XmlParser::XmlParser(std::string path, Camera *cam) {
    this->path = path;
    this->cam = cam;
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
    nPointLights = 0;
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
        } else if (entType == "Planet")
        {
            xml_node<> *planet = ent->first_node("Planet");
            _ents.push_back(getPlanet(planet));
        }
        entIndex++;
    }
}

Planet *XmlParser::getPlanet(xml_node<> *planet) {
    string type = planet->first_attribute("type")->value();
    bool hasAtmos = stoi(planet->first_attribute("hasAtmos")->value());
    float radius = stof(planet->first_attribute("radius")->value());
    int nSeg = stoi(planet->first_attribute("nSeg")->value());
    glm::vec3 position = getValues3(planet->first_node("Position"));
    xml_node<> * noiseSettings = planet->first_node("NoiseSettings");
    //-------NOISE SETTINGS-------
    float maxHeight = stof(noiseSettings->first_attribute("maxHeight")->value());
    float noiseFreq = stof(noiseSettings->first_attribute("noiseFreq")->value());
    int octaves = stoi(noiseSettings->first_attribute("octaves")->value());
    float lacunarity = stof(noiseSettings->first_attribute("lacunarity")->value());
    float fGain = stof(noiseSettings->first_attribute("fGain")->value());
    float fWeStr = stof(noiseSettings->first_attribute("fWeStr")->value());
    float fPinPonStr = stof(noiseSettings->first_attribute("fPinPonStr")->value());
    float cellJitter = stof(noiseSettings->first_attribute("cellJitter")->value());
    float domWarpAmp= stof(noiseSettings->first_attribute("domWarpAmp")->value());
    float minValue = stof(noiseSettings->first_attribute("minValue")->value());
    int noiseTypeSel = stoi(noiseSettings->first_attribute("noiseTypeSel")->value());
    int fractalTypeSel = stoi(noiseSettings->first_attribute("fractalTypeSel")->value());
    int cellDistTypeSel = stoi(noiseSettings->first_attribute("cellDistTypeSel")->value());
    int cellReturnTypeSel = stoi(noiseSettings->first_attribute("cellReturnTypeSel")->value());
    int domWarpTypeSel = stoi(noiseSettings->first_attribute("domWarpTypeSel")->value());
    //-----TEXTURES-------
    std::vector<string> pathDiffuse;
    std::vector<string> pathNormal;
    xml_node<> * textures = planet->first_node("Textures");
    xml_node<> * diffPath = textures->first_node("Diffuse");
    xml_node<> * normPath = textures->first_node("Normal");
    for (xml_node<> * d = diffPath->first_node("d");d;d = d->next_sibling())
    {
        pathDiffuse.push_back(d->value());
    }
    for (xml_node<> * n = normPath->first_node("n");n;n = n->next_sibling())
    {
        pathNormal.push_back(n->value());
    }
    Planet * newPlanet = new Planet(radius,nSeg,hasAtmos,maxHeight,noiseFreq,octaves,
                                    lacunarity,fGain,fWeStr,fPinPonStr,cellJitter,
                                    domWarpAmp,minValue,noiseTypeSel,fractalTypeSel,
                                    cellDistTypeSel,cellReturnTypeSel,domWarpTypeSel,
                                    pathDiffuse,pathNormal);
    //-------ATMOS SETTINGS-------
    xml_node<> * atmosSettings = planet->first_node("AtmosSettings");
    float atmosRadius = stof(atmosSettings->first_attribute("radius")->value());
    float kr = stof(atmosSettings->first_attribute("k_r")->value());
    float km = stof(atmosSettings->first_attribute("k_m")->value());
    float e = stof(atmosSettings->first_attribute("e")->value());
    float h = stof(atmosSettings->first_attribute("H")->value());
    float l = stof(atmosSettings->first_attribute("L")->value());
    float gm = stof(atmosSettings->first_attribute("g_m")->value());
    float numOutScatter = stof(atmosSettings->first_attribute("numOutScatter")->value());
    float numInScatter = stof(atmosSettings->first_attribute("numInScatter")->value());
    float scale = stof(atmosSettings->first_attribute("scale")->value());
    glm::vec3 color = getValues3(atmosSettings->first_node("Color"));
    newPlanet->addCamera(cam);
    newPlanet->setupAtmosphere(atmosRadius,kr,km,e,h,l,gm,numOutScatter,numInScatter,
                               scale,color);
    return newPlanet;
}

Light* XmlParser::getLight(xml_node<> *light) {

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
            auto lightAux = new Light(typePre,ambient,diffuse,specular,constant,linear,quadratic,entIndex,nPointLights);
            nPointLights++;
            lightAux->setPosition(position);
            return lightAux;
        } else if (typePre == "dirLight")
        {
            xml_node<> * dir = light->first_node("Direction");

            glm::vec3 direction = getValues3(dir);
            auto lightAux = new Light(typePre,ambient,diffuse,specular,direction,entIndex);
            lightAux->setDirection(direction);
            return lightAux;
        }
}

PhysicsObject* XmlParser::getObject(xml_node<> * model) {
    std::string typePre = model->first_attribute("type")->value();

    xml_node<> * pos = model->first_node("Position");
    xml_node<> * rot = model->first_node("Rotation");
    xml_node<> * scale = model->first_node("Scale");


    if (typePre == "Cube") {
        const char * path = (const char * )model->first_attribute("path")->value();
        std::string directory = model->first_attribute("directory")->value();
        Cube cube;
        Mesh *cubeMesh = procesMesh(cube.vertices, cube.indices,path,directory,192,36);
        Model *modelRes = new Model();
        modelRes->meshes.push_back(cubeMesh);
        auto *obj = new PhysicsObject(entIndex,1,modelRes);
        obj->setPosition(getValues3(pos));
        glm::vec4 rotation = getValues4(rot);
        obj->setRotation(rotation);
        obj->setScale(getValues3(scale));
        return obj;
    }
    else if (typePre == "AModel")
    {
        const char * path = (const char * )model->first_attribute("path")->value();
        auto obj = new PhysicsObject(entIndex,1,path);
        obj->setPosition(getValues3(pos));
        glm::vec4 rotation = getValues4(rot);
        obj->setRotation(rotation);
        obj->setScale(getValues3(scale));
        return obj;
    }
}

vector<Mesh*> XmlParser::getSprites() {
    std::vector<Mesh*> res;
    xml_node<> * models = _rootNode->first_node("Effects");

    //Renderer model parsing
    int i = 0;
    for (xml_node<> * effect = models->first_node("Effect"); effect; effect = effect->next_sibling() )
    {
        std::string typePre = effect->first_attribute("type")->value();

        xml_node<> * pos = effect->first_node("Position");
        xml_node<> * rot = effect->first_node("Rotation");
        xml_node<> * scale = effect->first_node("Scale");

        Mesh *cubeMesh;
        if (typePre == "Sprite" || typePre == "Window") {
            const char * path = (const char * )effect->first_attribute("path")->value();
            std::string directory = effect->first_attribute("directory")->value();
            Quad quad;
            cubeMesh = procesMesh(quad.vertices, quad.indices,path,directory,32,6);
        }

        cubeMesh->position = getValues3(pos);
        glm::vec4 rotation = getValues4(rot);
        cubeMesh->axis= glm::vec3(rotation);
        cubeMesh->angle = rotation.w;
        cubeMesh->scale = getValues3(scale);

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




