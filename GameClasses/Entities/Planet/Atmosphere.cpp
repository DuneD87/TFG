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
    Cubesphere cubesphere(atmosRadius,7,true);
    cubesphere.setupNoise(0,NULL);

    Mesh *modelMesh = new Mesh(cubesphere.vertexList,cubesphere.getIndices(),"","");
    skyDome = new Model();
    skyDome->meshes.push_back(modelMesh);
    _position = position;
    atmosShader = Shader("../Shaders/lightingShaderVertex.shader", "../Shaders/skydomeFragment.shader");

}

void Atmosphere::draw(Shader &shader, bool outlined, int depthMap) {
    renderGui();
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glFrontFace(GL_CW);
    atmosShader.use();
    //view
    glm::mat4 view = this->cam->GetViewMatrix();;
    atmosShader.setMat4("view",view);

    /*glm::mat4 tr = glm::mat4(1);
    tr = glm::translate(tr,cam->Position+cam->Front);
    glm::mat4 modelmat = glm::mat4(1);
    modelmat = glm::translate(modelmat,glm::vec3(0));
    glm::mat4 tv = glm::transpose(modelmat*tr);
    glm::vec4 camMult = {-tv[3][0], -tv[3][1], -tv[3][2], -tv[3][3]};
    glm::vec4 camPosition =tv * camMult;*/

    //projection
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(this->cam->Zoom), (float)3840/2160, 0.1f, 10000000.0f);
    atmosShader.setMat4("projection",projection);
    glm::mat4 cameraModel(1.0f);
    atmosShader.setMat4("model", cameraModel);

    //atmosShader.setFloat("cameraNear",0.1);
    //atmosShader.setFloat("cameraFar",10000000.0f);
    //atmosShader.setVec3("cameraForward",cam->Front);
    atmosShader.setVec3("cameraPosition",cam->Position-cam->Front*100.0f);
    //atmosShader.setMat4("inverseProjection",glm::inverse(projection));
    //atmosShader.setMat4("inverseView",glm::inverse(view));
    atmosShader.setVec3("planetPosition",_position);
    atmosShader.setFloat("planetRadius",planetRadius);
    atmosShader.setFloat("atmosRadius",atmosRadius);

    atmosShader.setFloat("K_R",k_r);
    atmosShader.setFloat("K_M",k_m);
    atmosShader.setFloat("E",e);
    atmosShader.setVec3("C_R",glm::normalize(glm::vec3(atmosColor[0],atmosColor[1],atmosColor[2])));
    atmosShader.setFloat("G_M",g_m);
    atmosShader.setFloat("MAX",viewDistance);

    atmosShader.setFloat("fNumOutScatter",numOutScatter);
    atmosShader.setFloat("fNumInScatter",numInScatter);

    skyDome->Draw(atmosShader,outlined,depthMap);
    glFrontFace(GL_CCW);
    skyDome->Draw(atmosShader,outlined,depthMap);
    glDisable(GL_BLEND);


    shader.use();
}

void Atmosphere::renderGui() {

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        ImGui::Begin("Atmospheric Settings",NULL,ImGuiWindowFlags_MenuBar);                          // Create a window called "Hello, world!" and append into it.
        ImGui::SetWindowFontScale(2);
        ImGui::PushItemWidth(200);

        ImGui::SliderFloat("outterRadius", &atmosRadius, 1000.0f, 10000.0f);

        ImGui::SliderFloat("innerRadius", &planetRadius, 1000.0f, 10000.0f);
        ImGui::NewLine();
        ImGui::SliderFloat("K_R", &k_r, -10.0f, 10.0f);
        ImGui::SliderFloat("K_M", &k_m, -10.0f, 10.0f);
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
