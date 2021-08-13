//
// Created by drive on 5/8/21.
//

#include "Atmosphere.h"
#include "Icosphere.h"
#include "Sphere.h"
#include "../../../libs/imgui_impl_opengl3.h"
#include "../../../libs/imgui_impl_glfw.h"

Atmosphere::Atmosphere(float planetRadius, float atmosRadius, Camera *cam,glm::vec3 position) {
    this->planetRadius = planetRadius;
    this->atmosRadius = atmosRadius;
    this->cam = cam;
    Cubesphere cubesphere(atmosRadius,4,true);
    cubesphere.setupNoise(0,NULL);

    Mesh *modelMesh = new Mesh(cubesphere.vertexList,cubesphere.getIndices(),"","");
    skyDome = new Model();
    skyDome->meshes.push_back(modelMesh);
    _position = position;
    atmosShader = Shader("../Shaders/lightingShaderVertex.shader", "../Shaders/skydomeFragment.shader");
    atmosShaderIN = Shader("../Shaders/lightingShaderVertex.shader", "../Shaders/backup/alternativeScatteringFragment.shader");
}

void Atmosphere::draw(Shader &shader, bool outlined, int depthMap) {
    renderGui();
    glEnable(GL_BLEND);
    //glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);

    glm::mat4 view = this->cam->GetViewMatrix();
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(this->cam->Zoom), (float)3840/2160, 0.1f, 10000000.0f);
    glm::mat4 cameraModel(1.0f);
    //std::cout<<"xPos: "<<cam->Position.x<<" yPos: "<<cam->Position.y<<" zPos: "<<cam->Position.z<<std::endl;
    //std::cout<<"xDir: "<<cam->orientation.x<<" yDir: "<<cam->orientation.y<<" zDir: "<<cam->orientation.z<<std::endl;
    atmosShader.use();
    atmosShader.setMat4("view",view);
    atmosShader.setMat4("projection",projection);
    atmosShader.setMat4("model", cameraModel);
    atmosShader.setVec3("cameraPosition",cam->Position);
    atmosShader.setVec3("cameraForward",cam->Front);
    atmosShader.setVec3("planetPosition",_position);
    atmosShader.setFloat("planetRadius",planetRadius);
    atmosShader.setFloat("atmosRadius",atmosRadius);
    atmosShader.setFloat("H",H);
    atmosShader.setFloat("L",L);
    atmosShader.setFloat("K_R",k_r);
    atmosShader.setFloat("K_M",k_m);
    atmosShader.setFloat("E",e);
    atmosShader.setVec3("C_R",glm::normalize(glm::vec3(atmosColor[0],atmosColor[1],atmosColor[2])));
    atmosShader.setFloat("G_M",g_m);
    atmosShader.setFloat("MAX",viewDistance);

    atmosShader.setFloat("fNumOutScatter",numOutScatter);
    atmosShader.setFloat("fNumInScatter",numInScatter);

    skyDome->Draw(atmosShader,outlined,depthMap);

    glFrontFace(GL_CW);
    atmosShader.setVec3("cameraPosition",cam->Position);
    //atmosShader.setMat4("inverseProjection",glm::inverse(projection));
    //atmosShader.setMat4("inverseView",glm::inverse(view));
    atmosShader.setVec3("planetPosition",_position);
    atmosShader.setFloat("planetRadius",planetRadius);
    //atmosShader.setFloat("cameraNear",0.1);
    //atmosShader.setFloat("cameraFar",10000000);
    atmosShader.setFloat("atmosRadius",atmosRadius);
    atmosShader.setMat4("view",view);
    atmosShader.setMat4("projection",projection);
    atmosShader.setMat4("model", cameraModel);
    skyDome->Draw(atmosShader,outlined,depthMap);

    glFrontFace(GL_CCW);
    glDisable(GL_BLEND);

    //glEnable(GL_CULL_FACE);
    shader.use();
}

void Atmosphere::renderGui() {

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        ImGui::Begin("Atmospheric Settings",NULL,ImGuiWindowFlags_MenuBar);                          // Create a window called "Hello, world!" and append into it.
        ImGui::SetWindowFontScale(1);
        ImGui::PushItemWidth(200);

        ImGui::SliderFloat("outterRadius", &atmosRadius, 0.0f, 100000.0f);
        ImGui::SliderFloat("innerRadius", &planetRadius, 0.0f, 100000.0f);
        ImGui::SliderFloat("viewDistance", &viewDistance, 0.0f, 10000000.0f);
        ImGui::NewLine();
        ImGui::SliderFloat("L", &L, 0.0f, 50.0f);
        ImGui::SliderFloat("H", &H, 0.0f, 50.0f);
        ImGui::SliderFloat("K_R", &k_r, -10.0f, 10.0f);
        ImGui::SliderFloat("K_M", &k_m, -2.0f, 0.1);
        ImGui::SliderFloat("E", &e, -100.0f, 100);
        ImGui::NewLine();
        ImGui::SliderFloat("G_M",&g_m,-100.0f,100.0f);
        ImGui::NewLine();
        ImGui::SliderFloat("numOutScatter",&numOutScatter,-10.0,500.0f);
        ImGui::SliderFloat("numInScatter",&numInScatter,-10.0f,500.0f);
        // Edit a color (stored as ~4 floats)
        ImGui::PushItemWidth(500);
        ImGui::ColorPicker3("Atmos color",atmosColor);
        ImGui::End();

        //if (test) setupMesh();
    }
}

Atmosphere::~Atmosphere() {
    delete skyDome;
}
