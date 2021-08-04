//
// Created by drive on 2/8/21.
//

#include "../../../libs/imgui_impl_opengl3.h"
#include "../../../libs/imgui_impl_glfw.h"
#include "Planet.h"
#include "Cubesphere.h"
#include "../../Util/FastNoiseLite.h"

Planet::Planet(float radius, int nSeg, glm::vec3 position) {
    this->type = 3;
    this->id = 34324;
    this->radius = radius;
    this->nSeg = nSeg;
    this->seed = time(NULL);
    _position = position;
    setupMesh();
}




void Planet::draw(Shader &shader, bool outlined, int depthMap) {

    shader.setInt("isTerrain",1);
    shader.setFloat("hPoint",highestPoint + hPointOffset);
    shader.setFloat("lPoint",lowestPoint - lPointOffset);
    shader.setFloat("pRadius",radius);
    shader.setVec3("upVector",_position);
    planet->Draw(shader,outlined,depthMap);
    shader.setInt("isTerrain",0);
    renderGui();
}

Planet::~Planet() {
    delete planet;
}

void Planet::renderGui() {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    bool test = false;
    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        ImGui::Begin("Planet Settings",NULL,ImGuiWindowFlags_MenuBar);                          // Create a window called "Hello, world!" and append into it.
        //ImGui::SetWindowFontScale(1.5);
        ImGui::PushItemWidth(200);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Highest Point offset");
        test = test + ImGui::SliderFloat("hPoint", &hPointOffset, 0.0f, 100.0f);
        ImGui::Text("Lowest Point offset");
        test =test + ImGui::SliderFloat("lPoint", &lPointOffset, 0.0f, 100.0f);
        ImGui::NewLine();
        test =test + ImGui::SliderFloat("Max height", &maxHeight, 0.0f, 10000.0f);
        test =test + ImGui::SliderFloat("Radius", &radius, 0.0f, 10000.0f);
        test =test + ImGui::SliderInt("Num of divisions", &nSeg, 0.0f, 10);
        ImGui::NewLine();
        test =test + ImGui::Combo("Noise Type",&noiseTypeSel,noiseTypes, IM_ARRAYSIZE(noiseTypes));
        test =test + ImGui::InputInt("Seed",&seed);
        test =test + ImGui::SliderFloat("Frequency",&noiseFreq,0.0f,0.1f);
        ImGui::NewLine();
        test =test + ImGui::Combo("Fractal Type",&fractalTypeSel, fractalTypes,IM_ARRAYSIZE(fractalTypes));
        test =test + ImGui::SliderInt("Octaves",&octaves,0.,64);
        test =test + ImGui::SliderFloat("Lacunarity",&lacunarity,0.0,5.0f);
        test =test + ImGui::SliderFloat("Fractal Gain",&fGain,0.0f,5.0f);
        test =test + ImGui::SliderFloat("Fractal Weighted Strength",&fWeStr,0.0f,5.0f);
        test =test +  ImGui::SliderFloat("Fractal Ping Pong Strength",&fPinPonStr,0.0f,5.0f);
        ImGui::NewLine();
        test =test +  ImGui::Combo("Cellular Distance Function",&cellDistTypeSel,cellDistFunc, IM_ARRAYSIZE(cellDistFunc));
        test =test +  ImGui::Combo("Cellular Return Type",&cellReturnTypeSel,cellReturnType, IM_ARRAYSIZE(cellDistFunc));
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



    auto *cubeSphere = new Cubesphere(radius,nSeg,true);
    cubeSphere->setupNoise(maxHeight,noise);
    //std::cout<<"Vertex count: "<<cubeSphere->vertexList.size()<<std::endl;
    std::vector<unsigned int> indices;
    const unsigned int * cubeIndex = cubeSphere->getIndices();
    for (int i = 0; i < cubeSphere->getIndexCount();i++)
        indices.push_back(cubeIndex[i]);
    highestPoint = cubeSphere->getHPoint();
    lowestPoint = cubeSphere->getLPoint();
    //std::cout<<"hPoint: "<<highestPoint+radius<<" lPoint: "<<lowestPoint-radius<<std::endl;
    const char *textures[] = {"grassy2.png","grassFlowers.png","mud.png","seamless_rock2.png"};
    delete planet;
    planet = new Mesh(cubeSphere->vertexList,indices,textures,4);
    delete cubeSphere;
}

