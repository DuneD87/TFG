//
// Created by drive on 2/8/21.
//

#include "../../Util/libs/imgui_impl_opengl3.h"
#include "Planet.h"
#include "../../Util/Cubesphere.h"

Planet::Planet(float radius, int nSeg, bool hasAtmos, float maxHeight,const std::vector<std::string> &path,
               glm::vec3 position, bool hasWater, FastNoiseLite Noise,int seed):
               radius(radius), nSeg(nSeg), hasAtmos(hasAtmos),
               maxHeight(maxHeight), path(path),hasWater(hasWater)
{
    _position = position;
    this->type = 3;
    drawEffects = true;
    this->seed = seed;
    bindPlanetTextures();
    this->noise = Noise;
    this->noise.SetSeed(this->seed);
    setupMesh();
}

void Planet::draw(Shader &shader) {
    renderGui();
    shader.use();
    shader.setInt("isTerrain",1);
    shader.setFloat("hPoint",highestPoint + hPointOffset);
    shader.setFloat("lPoint",lowestPoint - lPointOffset);
    shader.setFloat("pRadius",radius);

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

    shader.setInt("nTextures",path.size());
    entityMesh->Draw(shader);
    shader.setInt("isTerrain",0);
    if (drawEffects)
    {
        if (hasWater)
            water->draw(shader);
        if (hasAtmos)
            skyDome->draw(shader);
    }
}

Planet::~Planet() {
    delete entityMesh;
    if (hasAtmos)
        delete skyDome;
    if (hasWater)
        delete water;
}

void Planet::renderGui() {

    bool test = false;
    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        std::string guiId = "Planet settings" + std::to_string(this->id);
        ImGui::Begin(guiId.c_str(),NULL,ImGuiWindowFlags_MenuBar);                          // Create a window called "Hello, world!" and append into it.
        ImGui::SetWindowFontScale(setting_fontSize);
        ImGui::PushItemWidth(200);
        ImGui::Text("Planet ID: %d",id);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::SliderFloat("minValue", &minValue,-1.0f, 1.0f);
        ImGui::Text("Highest Point offset");
        ImGui::SliderFloat("hPoint", &hPointOffset, -1000.0f, 1000.0f);
        ImGui::Text("Lowest Point offset");
        ImGui::SliderFloat("lPoint", &lPointOffset, -1000.0f, 1000.0f);
        ImGui::NewLine();
        ImGui::SliderFloat("Max height", &maxHeight, -10000.0f, 10000.0f);
        ImGui::SliderFloat("Radius", &radius, 0.0f, 100000.0f);
        ImGui::SliderInt("Num of divisions", &nSeg, 0.0f, 20);
        ImGui::SliderFloat("DepthBlend", &depthBlend, -1.0f, 1.0f);
        ImGui::SliderFloat("BlendFactor", &blendFactor, -1.0f, 1.0f);
        ImGui::NewLine();
        ImGui::Combo("Noise Type",&noiseTypeSel,noiseTypes, IM_ARRAYSIZE(noiseTypes));
        ImGui::InputInt("Seed",&seed);
        ImGui::InputFloat("Frequency",&noiseFreq,0.0001);
        ImGui::NewLine();
        ImGui::Combo("Fractal Type",&fractalTypeSel, fractalTypes,IM_ARRAYSIZE(fractalTypes));
        ImGui::SliderInt("Octaves",&octaves,0.,64);
        ImGui::SliderFloat("Lacunarity",&lacunarity,0.0,5.0f);
        ImGui::SliderFloat("Fractal Gain",&fGain,0.0f,5.0f);
        ImGui::SliderFloat("Fractal Weighted Strength",&fWeStr,0.0f,5.0f);
        ImGui::SliderFloat("Fractal Ping Pong Strength",&fPinPonStr,0.0f,5.0f);
        ImGui::NewLine();
        ImGui::Combo("Cellular Distance Function",&cellDistTypeSel,cellDistFunc, IM_ARRAYSIZE(cellDistFunc));
        ImGui::Combo("Cellular Return Type",&cellReturnTypeSel,cellReturnType, IM_ARRAYSIZE(cellReturnType));
        ImGui::SliderFloat("Cellular Jitter",&cellJitter,0.0f,10.0f);
        ImGui::Combo("Domain Warp Type",&domWarpTypeSel,domainWarpType, IM_ARRAYSIZE(domainWarpType));
        ImGui::SliderFloat("Domain Warp Amplitude",&domWarpAmp,0.0f,50.0f);
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
        if (ImGui::Button("Rebuild")) setupMesh();
        if (!hasAtmos)
            ImGui::End();
        else
            skyDome->renderGui();
    }
}

void Planet::setupMesh() {

    auto *cubeSphere = new Cubesphere(radius,nSeg,true);
    cubeSphere->setupNoise(maxHeight,noise,minValue);
    highestPoint = cubeSphere->getHPoint();
    lowestPoint = cubeSphere->getLPoint();
    entityMesh = new Mesh(cubeSphere->getVertexList(),cubeSphere->getIndices(),textures);
    entityMesh->position = _position;
    delete cubeSphere;
}

float Planet::getRadius() const {
    return radius;
}

float Planet::getHighestPoint() const {
    return highestPoint;
}

float Planet::getLowestPoint() const {
    return lowestPoint;
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
}

void Planet::addCamera(Camera *cam) {
    this->cam = cam;
}

void Planet::setupEffects(float atmosRadius, float kr, float km, float e, float h, float l, float gm, float numOutScatter,
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
    if (water != NULL && hasWater)
        water->setWaterTexture(waterid);
}

void Planet::setNoDrawEffects(bool drawEffects) {
    this->drawEffects = drawEffects;
}

void
Planet::setupNoiseVariables(float noiseFreq, int octaves, float lacunarity, float fGain, float fWeStr, float fPinPonStr,
                            float cellJitter, float domWarpAmp, float minValue, int noiseTypeSel, int fractalTypeSel,
                            int cellDistTypeSel, int cellReturnTypeSel, int domWarpTypeSel) {
    this->noiseTypeSel = noiseTypeSel;
    this->noiseFreq = noiseFreq;
    this->cellDistTypeSel = cellDistTypeSel;
    this->cellReturnTypeSel = cellReturnTypeSel;
    this->cellJitter = cellJitter;
    this->domWarpTypeSel = domWarpTypeSel;
    this->domWarpAmp = domWarpAmp;
    this->fractalTypeSel = fractalTypeSel;
    this->octaves = octaves;
    this->lacunarity = lacunarity;
    this->fGain = fGain;
    this->fWeStr = fWeStr;
    this->fPinPonStr = fPinPonStr;
}
