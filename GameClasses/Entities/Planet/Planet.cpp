//
// Created by drive on 2/8/21.
//

#include <libs/imgui_impl_opengl3.h>
#include <libs/imgui_impl_glfw.h>
#include "Planet.h"
#include "Cubesphere.h"
#include "../../Util/FastNoiseLite.h"

Planet::Planet(float radius, int nSeg, bool smoot) {
    this->type = 3;
    this->id = 34324;
    this->radius = radius;
    _position = glm::vec3(0);
    const char *textures[] = {"grassy2.png","grassFlowers.png","mud.png","seamless_rock2.png"};
    //up,down,left,right,front,back
    auto *cubeSphere = new Cubesphere(radius,nSeg,true);
    std::cout<<"Vertex count: "<<cubeSphere->vertexList.size()<<std::endl;
    std::vector<unsigned int> indices;
    const unsigned int * cubeIndex = cubeSphere->getIndices();
    for (int i = 0; i < cubeSphere->getIndexCount();i++)
        indices.push_back(cubeIndex[i]);
    highestPoint = cubeSphere->getHPoint();
    lowestPoint = cubeSphere->getLPoint();
    std::cout<<"hPoint: "<<highestPoint+radius<<" lPoint: "<<lowestPoint-radius<<std::endl;
    planet = new Mesh(cubeSphere->vertexList,indices,textures,4);
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
    for (auto face: faces)
        delete face;
}

void Planet::renderGui() {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static int noiseTypeSel = 0, fractalTypeSel = 0,cellDistTypeSel = 0, cellReturnTypeSel = 0,
    domWarpTypeSel = 0;
    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        ImGui::Begin("Planet Settings",NULL,ImGuiWindowFlags_MenuBar);                          // Create a window called "Hello, world!" and append into it.
        ImGui::SetWindowFontScale(1.5);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Highest Point offset");
        ImGui::SliderFloat("hPoint", &hPointOffset, 0.0f, 100.0f);
        ImGui::Text("Lowest Point offset");
        ImGui::SliderFloat("lPoint", &lPointOffset, 0.0f, 100.0f);
        ImGui::NewLine();

        ImGui::Combo("Noise Type",&noiseTypeSel,noiseTypes, IM_ARRAYSIZE(noiseTypes));
        ImGui::InputFloat("Seed",&seed);
        ImGui::SliderFloat("Frequency",&noiseFreq,0.0f,100.0f);
        ImGui::NewLine();
        ImGui::Combo("Fractal Type",&fractalTypeSel, fractalTypes,IM_ARRAYSIZE(fractalTypes));
        ImGui::SliderFloat("Octaves",&octaves,0.0f,64.0f);
        ImGui::SliderFloat("Lacunarity",&lacunarity,0.0,5.0f);
        ImGui::SliderFloat("Fractal Gain",&fGain,0.0f,5.0f);
        ImGui::SliderFloat("Fractal Weighted Strength",&fWeStr,0.0f,5.0f);
        ImGui::SliderFloat("Fractal Ping Pong Strength",&fPinPonStr,0.0f,5.0f);
        ImGui::NewLine();
        ImGui::Combo("Cellular Distance Function",&cellDistTypeSel,cellDistFunc, IM_ARRAYSIZE(cellDistFunc));
        ImGui::Combo("Cellular Return Type",&cellReturnTypeSel,cellDistFunc, IM_ARRAYSIZE(cellDistFunc));
        ImGui::SliderFloat("Cellular Jitter",&cellJitter,0.0f,10.0f);
        ImGui::Combo("Domain Warp Type",&domWarpTypeSel,domainWarpType, IM_ARRAYSIZE(domainWarpType));
        ImGui::SliderFloat("Domain Warp Amplitude",&domWarpAmp,0.0f,50.0f);
        ImGui::End();

    }

}

