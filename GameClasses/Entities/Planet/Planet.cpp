//
// Created by drive on 2/8/21.
//

#include "../../../libs/imgui_impl_opengl3.h"
#include "../../../libs/imgui_impl_glfw.h"
#include "Planet.h"
#include "Cubesphere.h"
#include "../../Util/FastNoiseLite.h"

Planet::Planet(float radius, int nSeg, glm::vec3 position, Camera *cam) {
    this->type = 3;
    this->id = 34324;
    this->radius = radius;
    this->nSeg = nSeg;
    this->seed = time(NULL);
    _position = position;
    this->cam = cam;
    setupMesh();
}




void Planet::draw(Shader &shader, bool outlined, int depthMap) {

    shader.setInt("isTerrain",1);
    shader.setFloat("hPoint",highestPoint + hPointOffset);
    shader.setFloat("lPoint",lowestPoint - lPointOffset);
    shader.setFloat("pRadius",radius);
    shader.setVec3("upVector",_position);
    shader.setFloat("blendFactor",blendFactor);
    shader.setFloat("depthBlend",depthBlend);
    planet->Draw(shader,outlined,depthMap);
    shader.setInt("isTerrain",0);
    skyDome->draw(shader,outlined,-1);

}

Planet::~Planet() {
    delete planet;
    delete skyDome;
}

void Planet::renderGui() {



    bool test = false;
    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        ImGui::Begin("Planet Settings",NULL,ImGuiWindowFlags_MenuBar);                          // Create a window called "Hello, world!" and append into it.
        ImGui::SetWindowFontScale(2);
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
    const char *textures[] = {"grassy2.png","grassFlowers.png","mud.png","seamless_rock2.png","snow.jpg"};
    const char *diffuse[] = {
            //DiffuseMap
            "./Planet/sandy-rocks1-albedo-1024.png",
            "./Planet/sandyground-albedo-1024.png",
            "./Planet/rough-wet-cobble-albedo-1024.png",
            "./Planet/grass1-albedo3-1024.png",
            "./Planet/dirt_01_diffuse-1024.png",
            "./Planet/worn-bumpy-rock-albedo-1024.png",
            "./Planet/rock-snow-ice-albedo-1024.png",
            "./Planet/snow-packed-albedo-1024.png",
            //NormalMap
            "./Planet/sandy-rocks1-normal-1024.jpg",
            "./Planet/sandyground-normal-1024.jpg",
            "./Planet/rough-wet-cobble-normal-1024.jpg",
            "./Planet/grass1-normal-1024.jpg",
            "./Planet/dirt_01_normal-1024.jpg",
            "./Planet/worn-bumpy-rock-normal-1024.jpg",
            "./Planet/rock-snow-ice-normal-1024.jpg",
            "./Planet/snow-packed-normal-1024.jpg",

              };
    delete planet;
    planet = new Mesh(cubeSphere->vertexList,indices,diffuse,8,0);
    delete cubeSphere;
    skyDome = new Atmosphere(radius-lowestPoint,(radius*1.1)+highestPoint,cam,_position);
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

