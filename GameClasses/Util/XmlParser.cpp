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
    nPlanets = 0;
    xml_node<> * shadersNode = _rootNode->first_node("Shaders");
    for (xml_node<> * shaderNode = shadersNode->first_node("Shader"); shaderNode; shaderNode = shaderNode->next_sibling("Shader"))
    {
        xml_node<> * shaderPath = shaderNode->first_node("Path");
        std::vector<std::string> paths;

        for (xml_node<> * p = shaderPath->first_node("p"); p; p = p->next_sibling("p"))
        {
            paths.push_back(std::string(p->value()));
        }
        if (paths.size() == 2)
        {
            //2 shaders, vertex and fragment
            Shader shader(paths[0].c_str(),paths[1].c_str());
            shader.setParserId(std::stoi(shaderNode->first_attribute("id")->value()));
            shaders.push_back(shader);
        } else if (paths.size() == 4)
        {
            Shader shader(paths[0].c_str(),paths[1].c_str(),paths[2].c_str(),paths[3].c_str());
            shader.setParserId(std::stoi(shaderNode->first_attribute("id")->value()));
            shaders.push_back(shader);
        }
    }
    std::cout<<"Shader count: "<<shaders.size()<<std::endl;
    xml_node<> * cameraNode = _rootNode->first_node("Camera");
    xml_node<> * camPosNode = cameraNode->first_node("Position");
    xml_node<> * camOrient = cameraNode->first_node("Orientation");

    cam->Position = getValues3(camPosNode);
    cam->Pitch = stof(camOrient->first_attribute("pitch")->value());
    cam->Yaw = stof(camOrient->first_attribute("yaw")->value());
    cam->Roll = stof(camOrient->first_attribute("roll")->value());
    cam->updateCameraVectors();
    xml_node<> * entities = _rootNode->first_node("Entities");
    for (xml_node<> * ent = entities->first_node("Entity");ent;ent = ent->next_sibling("Entity"))
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
            nPlanets++;
        }
        entIndex++;
    }
    theFile.close();
}

void XmlParser::readData() {

}


Planet *XmlParser::getPlanet(xml_node<> *planet) {
    int id = stoi(planet->first_attribute("id")->value());
    int shaderId = stoi(planet->first_attribute("shaderId")->value());
    bool hasAtmos = stoi(planet->first_attribute("hasAtmos")->value());
    bool hasWater = stoi(planet->first_attribute("hasWater")->value());
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
                                    pathDiffuse,pathNormal,position,hasWater);
    newPlanet->id = id;
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
    newPlanet->entityShader = shaders[shaderId];
    if (hasAtmos)
        newPlanet->setupAtmosphere(atmosRadius,kr,km,e,h,l,gm,numOutScatter,numInScatter,
                                    scale,color);
    xml_node<> * biomeRoot = planet->first_node("Biomes");
    for (xml_node<> * bio = biomeRoot->first_node("Biome");bio;bio = bio->next_sibling())
    {
        Biome bioAux;
        bioAux.latStart = stof(bio->first_attribute("latStart")->value());
        bioAux.latEnd = stof(bio->first_attribute("latEnd")->value());
        xml_node<> * textIndex = bio->first_node("Textures");

        for (xml_node<>  * textNode = textIndex->first_node("Texture");textNode;textNode = textNode->next_sibling("Texture"))
        {
            TextHeight textHeight;
            textHeight.index = stoi(textNode->first_attribute("index")->value());
            textHeight.hStart = stof(textNode->first_attribute("hStart")->value());
            textHeight.hEnd = stof(textNode->first_attribute("hEnd")->value());
            bioAux.textHeight.push_back(textHeight);
        }
        newPlanet->addBiome(bioAux);

    }
    //newPlanet->computeBiomes();
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
            xml_node<> * dir = light->first_node("Orientation");
            float sunPitch = stof(dir->first_attribute("pitch")->value());
            float sunYaw = stof(dir->first_attribute("yaw")->value());

            glm::vec2 orientation(sunPitch,sunYaw);
            auto lightAux = new Light(typePre,ambient,diffuse,specular,orientation,entIndex);
            lightAux->setDirection(orientation);
            this->sun = lightAux;
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

vector<string> XmlParser::split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

void XmlParser::saveWorld() {

    xml_document<> doc;
    // Read the xml file into a vector
    ifstream theFile (this->path);
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    // Parse the buffer using the xml file parsing library into doc
    doc.parse<parse_full | parse_no_data_nodes>(&buffer[0]);
    // Find our root node
    _rootNode = doc.first_node("Scene");
    xml_node<> * camNode = _rootNode->first_node("Camera");
    xml_node<> * camNodePos = camNode->first_node("Position");

    std::string x = std::to_string(cam->Position.x);
    std::string y = std::to_string(cam->Position.y);
    std::string z = std::to_string(cam->Position.z);
    camNodePos->first_attribute("x")->value(doc.allocate_string(x.c_str()));
    camNodePos->first_attribute("y")->value(doc.allocate_string(y.c_str()));
    camNodePos->first_attribute("z")->value(doc.allocate_string(z.c_str()));
    xml_node<> * camOrientation = camNode->first_node("Orientation");
    cam->updateCameraVectors();
    std::string x1 = std::to_string(cam->oldPitch);
    std::string y1 = std::to_string(cam->oldYaw);
    std::string z1 = std::to_string(cam->oldRoll);
    camOrientation->first_attribute("pitch")->value(doc.allocate_string(x1.c_str()));
    camOrientation->first_attribute("yaw")->value(doc.allocate_string(y1.c_str()));
    camOrientation->first_attribute("roll")->value(doc.allocate_string(z1.c_str()));
    xml_node<> * ents = _rootNode->first_node("Entities");
    for (xml_node<> * ent = ents->first_node("Entity");ent;ent = ent->next_sibling("Entity"))
    {
        string entType = ent->first_attribute("type")->value();
        if (entType == "Light")
        {
            xml_node<> *light = ent->first_node("Light");
            string typePre = light->first_attribute("type")->value();
            if (typePre == "dirLight")
            {
                xml_node<> * dir = light->first_node("Orientation");

                string sunSettings = sun->toString();
                vector<string> res = split(sunSettings,":");
                vector<string> col = split(res[1],",");
                dir->first_attribute("pitch")->value(doc.allocate_string(col[0].c_str()));
                dir->first_attribute("yaw")->value(doc.allocate_string(col[1].c_str()));
            }

        } else if (entType == "Planet")
        {
            xml_node<> *planet = ent->first_node("Planet");
            int id = stoi(planet->first_attribute("id")->value());

            for (auto _ent : _ents)
            {
                if (_ent->id == id && _ent->getType() == 3)
                {

                    xml_node<>* noiseSettings  = planet->first_node("NoiseSettings");
                    Planet * auxPlanet = dynamic_cast<Planet*>(_ent);
                    string pSettings = auxPlanet->toString();
                    vector<string> res = split(pSettings,"\n");
                    for (const auto& attr : res)
                    {
                        vector<string> namVal = split(attr,":");
                        const char* name = namVal[0].c_str();
                        const char* val = doc.allocate_string(namVal[1].c_str());
                        noiseSettings->first_attribute(name)->value(val);

                    }
                    if (auxPlanet->hasAtmosphere())
                    {
                        string atmosSettings = auxPlanet->getAtmosSettings();
                        vector<string> atmosRes = split(atmosSettings,"\n");
                        for (const auto& attr : atmosRes)
                        {
                            xml_node<> * atmosSettingsNode = planet->first_node("AtmosSettings");
                            vector<string> namVal = split(attr,":");
                            const char * name = namVal[0].c_str();
                            const char * val = namVal[1].c_str();
                            if (namVal[0] != "color")
                            {
                                atmosSettingsNode->first_attribute(name)->value(doc.allocate_string(val));
                            }
                            else
                            {
                                vector<string> colorVal = split(val,",");
                                xml_node<> * colorNode = atmosSettingsNode->first_node("Color");
                                colorNode->first_attribute("x")->value(doc.allocate_string(colorVal[0].c_str()));
                                colorNode->first_attribute("y")->value(doc.allocate_string(colorVal[1].c_str()));
                                colorNode->first_attribute("z")->value(doc.allocate_string(colorVal[2].c_str()));

                            }
                        }
                    }
                }
            }
        }
    }
    ofstream file;
    file.open(path.c_str());
    std::string data;
    rapidxml::print(std::back_inserter(data),doc);
    file << data;
    file.close();
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

void XmlParser::setValues3(xml_node<> *origin, std::vector<string> dir) {
    origin->first_attribute("x")->value(dir[0].c_str());
    origin->first_attribute("y")->value(dir[1].c_str());
    origin->first_attribute("z")->value(dir[2].c_str());
}

XmlParser::~XmlParser() {

}







