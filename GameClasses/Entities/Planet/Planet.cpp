//
// Created by drive on 2/8/21.
//

#include "../../Util/libs/imgui_impl_opengl3.h"
#include "Planet.h"
#include "../../Util/Cubesphere.h"
#include "../PhysicsObject.h"

Planet::Planet(float radius, int nSeg, glm::vec3 position, Camera *cam, std::vector<std::string> path) {
    this->type = 3;
    this->id = 34324;
    this->radius = radius;
    this->nSeg = nSeg;
    this->seed = time(NULL);
    _position = position;
    this->cam = cam;
    this->path = path;
    bindPlanetTextures();
    setupMesh();
    //addVegetation();
}
Planet::Planet(float radius, int nSeg, bool hasAtmos, float maxHeight, float noiseFreq, int octaves, float lacunarity,
               float fGain, float fWeStr, float fPinPonStr, float cellJitter, float domWarpAmp, float minValue,
               int noiseTypeSel, int fractalTypeSel, int cellDistTypeSel, int cellReturnTypeSel, int domWarpTypeSel,
               const std::vector<std::string> &path, const std::vector<std::string> &pathNorm,glm::vec3 position, bool hasWater):radius(radius), nSeg(nSeg), hasAtmos(hasAtmos),
                                                       maxHeight(maxHeight), noiseFreq(noiseFreq), octaves(octaves),
                                                       lacunarity(lacunarity), fGain(fGain), fWeStr(fWeStr),
                                                       fPinPonStr(fPinPonStr), cellJitter(cellJitter),
                                                       domWarpAmp(domWarpAmp), minValue(minValue),
                                                       noiseTypeSel(noiseTypeSel), fractalTypeSel(fractalTypeSel),
                                                       cellDistTypeSel(cellDistTypeSel),
                                                       cellReturnTypeSel(cellReturnTypeSel),
                                                       domWarpTypeSel(domWarpTypeSel), path(path),
                                                       pathNormal(pathNorm), hasWater(hasWater)
{
    _position = position;
    this->type = 3;

    bindPlanetTextures();
    setupMesh();
}




void Planet::draw(Shader &shader, bool outlined, int depthMap) {

    shader.use();
    shader.setFloat("near",setting_near);
    shader.setFloat("far",setting_far);
    shader.setInt("isTerrain",1);
    shader.setFloat("hPoint",highestPoint + hPointOffset);
    shader.setFloat("lPoint",lowestPoint - lPointOffset);
    shader.setFloat("pRadius",radius);
    shader.setFloat("blendFactor",blendFactor);
    shader.setFloat("depthBlend",depthBlend);
    shader.setVec3("pPosition",_position);
    shader.setInt("nBiomes",biomes.size());
    shader.setInt("nTextures",path.size());

    for (int i = 0; i < biomes.size();i++)
    {
        shader.setFloat("biomes["+std::to_string(i)+"].latStart",biomes[i].latStart);
        shader.setFloat("biomes["+std::to_string(i)+"].latEnd",biomes[i].latEnd);
        shader.setInt("biomes["+std::to_string(i)+"].nTextBiome",biomes[i].textHeight.size());
        for (int j = 0; j < biomes[i].textHeight.size();j++)
        {
            shader.setInt("biomes["+std::to_string(i)+"].textIndex["+std::to_string(j)+"].index",biomes[i].textHeight[j].index);
            shader.setFloat("biomes["+std::to_string(i)+"].textIndex["+std::to_string(j)+"].hStart",biomes[i].textHeight[j].hStart);
            shader.setFloat("biomes["+std::to_string(i)+"].textIndex["+std::to_string(j)+"].hEnd",biomes[i].textHeight[j].hEnd);
        }
    }

    /*if (!biomeSet)
    {
        for (int i = 0; i < biomes.size();i++)
        {
            std::cout<<"Biome: "<<i<<" LAT START: " << biomes[i].latStart<<" ----- LAT END: " << biomes[i].latEnd<<std::endl;
            std::cout<<"nTextures: "<<biomes[i].textHeight.size()<<std::endl;
            for (int j = 0; j < biomes[i].textHeight.size();j++)
            {
                std::cout<<"Texture index: "<<biomes[i].textHeight[j].index<<std::endl;
                std::cout<<"hStart: "<<biomes[i].textHeight[j].hStart<<std::endl;
                std::cout<<"hEnd: "<<biomes[i].textHeight[j].hEnd<<std::endl;
                std::cout<<"------------------------------------------------"<<std::endl;
            }
            std::cout<<"****************************************************"<<std::endl;
        }
        biomeSet = true;
    }*/

    shader.setInt("nTextures",path.size());
    planet->Draw(shader,outlined,depthMap,true,true);
    shader.setInt("isTerrain",0);
    if (hasWater)
        water->draw(shader,outlined,depthMap);
    if (hasAtmos)
        skyDome->draw(shader,outlined,-1);

}

Planet::~Planet() {
    delete planet;
    if (hasAtmos)
        delete skyDome;
}

void Planet::renderGui() {

    bool test = false;
    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        ImGui::Begin("Planet Settings" + this->id,NULL,ImGuiWindowFlags_MenuBar);                          // Create a window called "Hello, world!" and append into it.
        ImGui::SetWindowFontScale(setting_fontSize);
        ImGui::PushItemWidth(200);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        test = test + ImGui::SliderFloat("minValue", &minValue,-1.0f, 1.0f);
        ImGui::Text("Highest Point offset");
        test = test + ImGui::SliderFloat("hPoint", &hPointOffset, -1000.0f, 1000.0f);
        ImGui::Text("Lowest Point offset");
        test =test + ImGui::SliderFloat("lPoint", &lPointOffset, -1000.0f, 1000.0f);
        ImGui::NewLine();
        test =test + ImGui::SliderFloat("Max height", &maxHeight, -10000.0f, 10000.0f);
        test =test + ImGui::SliderFloat("Radius", &radius, 0.0f, 100000.0f);
        test =test + ImGui::SliderInt("Num of divisions", &nSeg, 0.0f, 20);
        test =test + ImGui::SliderFloat("DepthBlend", &depthBlend, -1.0f, 1.0f);
        test =test + ImGui::SliderFloat("BlendFactor", &blendFactor, -1.0f, 1.0f);
        ImGui::NewLine();
        test =test + ImGui::Combo("Noise Type",&noiseTypeSel,noiseTypes, IM_ARRAYSIZE(noiseTypes));
        test =test + ImGui::InputInt("Seed",&seed);
        test =test + ImGui::InputFloat("Frequency",&noiseFreq,0.0001);
        ImGui::NewLine();
        test =test + ImGui::Combo("Fractal Type",&fractalTypeSel, fractalTypes,IM_ARRAYSIZE(fractalTypes));
        test =test + ImGui::SliderInt("Octaves",&octaves,0.,64);
        test =test + ImGui::SliderFloat("Lacunarity",&lacunarity,0.0,5.0f);
        test =test + ImGui::SliderFloat("Fractal Gain",&fGain,0.0f,5.0f);
        test =test + ImGui::SliderFloat("Fractal Weighted Strength",&fWeStr,0.0f,5.0f);
        test =test +  ImGui::SliderFloat("Fractal Ping Pong Strength",&fPinPonStr,0.0f,5.0f);
        ImGui::NewLine();
        test =test +  ImGui::Combo("Cellular Distance Function",&cellDistTypeSel,cellDistFunc, IM_ARRAYSIZE(cellDistFunc));
        test =test +  ImGui::Combo("Cellular Return Type",&cellReturnTypeSel,cellReturnType, IM_ARRAYSIZE(cellReturnType));
        test =test +  ImGui::SliderFloat("Cellular Jitter",&cellJitter,0.0f,10.0f);
        test =test +  ImGui::Combo("Domain Warp Type",&domWarpTypeSel,domainWarpType, IM_ARRAYSIZE(domainWarpType));
        test =test +  ImGui::SliderFloat("Domain Warp Amplitude",&domWarpAmp,0.0f,50.0f);
        if (ImGui::Button("Rebuild")) setupMesh();
        ImGui::End();

        //if (test) setupMesh();
    }

}

void Planet::setupMesh() {

    FastNoiseLite noise;

    noise.SetSeed(seed);
    noise.SetNoiseType(_noiseTypes[noiseTypeSel]);
    noise.SetFrequency(noiseFreq);

    noise.SetCellularDistanceFunction(_cellDistFunc[cellDistTypeSel]);
    noise.SetCellularReturnType(_cellReturnType[cellReturnTypeSel]);
    noise.SetCellularJitter(cellJitter);
    noise.SetDomainWarpType(_domWarpType[domWarpTypeSel]);
    noise.SetDomainWarpAmp(domWarpAmp);

    noise.SetFractalType(_fractalTypes[fractalTypeSel]);
    noise.SetFractalOctaves(octaves);
    noise.SetFractalLacunarity(lacunarity);
    noise.SetFractalGain(fGain);
    noise.SetFractalWeightedStrength(fWeStr);
    noise.SetFractalPingPongStrength(fPinPonStr);



    auto *cubeSphere = new Cubesphere(radius,nSeg,true,minValue);
    cubeSphere->setupNoise(maxHeight,noise,minValue);
    //std::cout<<"Vertex count: "<<cubeSphere->vertexList.size()<<std::endl;
    std::vector<unsigned int> indices = cubeSphere->getIndices();
    highestPoint = cubeSphere->getHPoint();
    lowestPoint = cubeSphere->getLPoint();
    std::cout<<"hPoint: "<<highestPoint<<" lPoint: "<<lowestPoint<<std::endl;

    planet = new Mesh(cubeSphere->vertexList,indices,textures);

    planet->position = _position;
    delete cubeSphere;

}

float Planet::getRadius() const {
    return radius;
}

float Planet::getHPointOffset() const {
    return hPointOffset;
}

float Planet::getLPointOffset() const {
    return lPointOffset;
}

float Planet::getMaxHeight() const {
    return maxHeight;
}

int Planet::getNSeg() const {
    return nSeg;
}

float Planet::getHighestPoint() const {
    return highestPoint;
}

float Planet::getLowestPoint() const {
    return lowestPoint;
}

void Planet::addVegetation() {
    srand(time(NULL));
    for (int i = 0; i < planet->vertices.size();i++)
    {
        int r = rand()%14;
        glm::vec3 position = planet->vertices[i].Position;
        PhysicsObject *ent = new PhysicsObject(-1,0,decoPaths[r]);
        ent->setPosition(position);
        planetDeco.push_back(ent);
    }
}

void Planet::bindPlanetTextures() {
    int nTextures = path.size();
    for (auto i = 0; i < nTextures; i++)
    {
        Texture text;
        text.path = "../Textures/" + path[i];
        text.type = "texture_diffuse";
        text.id = TextureFromFile( path[i].c_str(),"../Textures/",false,true);
        textures.push_back(text);
    }
    //Normal
    for (auto i = 0; i < nTextures ; i++)
    {
        Texture text;
        text.path = "../Textures/" + pathNormal[i];
        text.type = "texture_normal";
        text.id = TextureFromFile( pathNormal[i].c_str(),"../Textures/",false,true);
        textures.push_back(text);
    }
}

void Planet::addCamera(Camera *cam) {
    this->cam = cam;
}

void Planet::setupAtmosphere(float atmosRadius, float kr, float km, float e, float h, float l, float gm, float numOutScatter,
                        float numInScatter, float scale, glm::vec3 color) {
    float colorAux[3];
    for (int i = 0; i < 3; i++)
        colorAux[i] = color[i];
    skyDome = new Atmosphere(atmosRadius-lowestPoint,atmosRadius+highestPoint,cam,kr,km,e,h,l,colorAux,gm,numOutScatter,numInScatter,scale,_position);

    if (hasWater)
        water = new WaterSphere(radius,radius,cam,_position);
}

void Planet::addBiome(Biome bio) {
    biomes.push_back(bio);
}

void Planet::setSun(Light* sun) {
    if (this->hasAtmos)
    {
        skyDome->setSun(sun);
        if (this->hasWater)
            water->setSun(sun);
    }

}

std::string Planet::toString() {
    std::stringstream ststr;
    ststr<< "maxHeight:"<<maxHeight<<"\n"
         << "noiseFreq:"<<noiseFreq<<"\n"
         << "octaves:"<<octaves<<"\n"
         << "lacunarity:"<<lacunarity<<"\n"
         << "fGain:"<<fGain<<"\n"
         << "fWeStr:"<<fWeStr<<"\n"
         << "fPinPonStr:"<<fPinPonStr<<"\n"
         << "cellJitter:"<<cellJitter<<"\n"
         << "domWarpAmp:"<<domWarpAmp<<"\n"
         << "noiseTypeSel:"<<noiseTypeSel<<"\n"
         << "fractalTypeSel:"<<fractalTypeSel<<"\n"
         << "cellDistTypeSel:"<<cellDistTypeSel<<"\n"
         << "cellReturnTypeSel:"<<cellReturnTypeSel<<"\n"
         << "domWarpTypeSel:"<<domWarpTypeSel;
    return ststr.str();
}

bool Planet::hasAtmosphere() const {
    return this->hasAtmos;
}

std::string Planet::getAtmosSettings() const {
    return skyDome->toString();
}

void Planet::setWaterTexture(unsigned int waterid) {
    if (water != NULL)
        water->setWaterTexture(waterid);
}
