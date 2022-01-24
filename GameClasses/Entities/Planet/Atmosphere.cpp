//
// Created by drive on 5/8/21.
//

#include "Atmosphere.h"

#include "../../Util/libs/imgui_impl_opengl3.h"

Atmosphere::Atmosphere(float planetRadius, float atmosRadius, Camera *cam, float kR, float kM, float e, float h,
                       float l, float atmosColor[3], float gM, float numOutScatter, float numInScatter, float scale,glm::vec3 position)
        : planetRadius(planetRadius), atmosRadius(atmosRadius), cam(cam), k_r(kR), k_m(kM), e(e), H(h), L(l),
          g_m(gM), numOutScatter(numOutScatter), numInScatter(numInScatter), scale(scale) {
    for (int i = 0; i < 3; i++)
        this->atmosColor[i] = atmosColor[i];
    this->planetRadius = planetRadius;
    this->atmosRadius = atmosRadius;
    this->_position = position;
    this->cam = cam;
    Cubesphere cubesphere(atmosRadius,4,true);
    cubesphere.setupNoise(0,NULL);
    entityMesh = new Mesh(cubesphere.vertexList,cubesphere.getIndices());
    entityMesh->position = position;
    entityShader = Shader("../Shaders/skydomeVertex.shader", "../Shaders/skydomeFragment.shader");
}

void Atmosphere::draw(Shader &shader) {
    //renderGui();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
    glm::mat4 view = this->cam->GetViewMatrix();
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(this->cam->Zoom), (float)setting_scrWidth/setting_scrHeight, setting_near, setting_far);
    glm::mat4 cameraModel(1.0f);
    entityShader.use();
    entityShader.setMat4("view",view);
    entityShader.setMat4("projection",projection);
    entityShader.setMat4("model", cameraModel);
    entityShader.setVec3("cameraPosition",cam->Position);
    entityShader.setVec3("cameraForward",cam->Front);
    entityShader.setVec3("planetPosition",_position);
    entityShader.setFloat("planetRadius",planetRadius);
    entityShader.setFloat("atmosRadius",atmosRadius);
    entityShader.setVec3("sunDir",sun->getDirVector());
    entityShader.setFloat("H",H);
    entityShader.setFloat("L",L);
    entityShader.setFloat("K_R",k_r);
    entityShader.setFloat("K_M",k_m);
    entityShader.setFloat("E",e);
    entityShader.setVec3("C_R",glm::normalize(glm::vec3(atmosColor[0],atmosColor[1],atmosColor[2])));
    entityShader.setFloat("G_M",g_m);
    entityShader.setFloat("MAX",viewDistance);
    entityShader.setFloat("fNumOutScatter",numOutScatter);
    entityShader.setFloat("fNumInScatter",numInScatter);
    glFrontFace(GL_CW);
    entityMesh->Draw(entityShader);
    glFrontFace(GL_CCW);
    entityMesh->Draw(entityShader);
    glDisable(GL_BLEND);
    shader.use();
}

void Atmosphere::renderGui() {

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        //ImGui::Begin("Atmospheric Settings",NULL,ImGuiWindowFlags_MenuBar);                          // Create a window called "Hello, world!" and append into it.
        ImGui::SetWindowFontScale(setting_fontSize);
        ImGui::PushItemWidth(200);

        ImGui::SliderFloat("outterRadius", &atmosRadius, 0.0f, 100000.0f);
        ImGui::SliderFloat("innerRadius", &planetRadius, 0.0f, 100000.0f);
        ImGui::SliderFloat("MAX", &viewDistance, 0.0f, 10000000.0f);
        ImGui::SliderFloat("Scale",&scale,1.0f,10.0f);
        entityMesh->scale = glm::vec3(scale);
        ImGui::NewLine();
        ImGui::SliderFloat("L", &L, 0.0f, 50.0f);
        ImGui::SliderFloat("H", &H, 0.0f, 50.0f);
        ImGui::SliderFloat("K_R", &k_r, -10.0f, 10.0f);
        ImGui::SliderFloat("K_M", &k_m, -2.0f, 2.0);
        ImGui::SliderFloat("E", &e, -100.0f, 100);
        ImGui::NewLine();
        ImGui::SliderFloat("G_M",&g_m,-5.0f,5.0f);
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
    delete entityMesh;
}


void Atmosphere::setSun(Light* sun) {
    this->sun = sun;
}

std::string Atmosphere::toString() {
   std::stringstream ststr;
   ststr<< "k_r:"<<k_r<<"\n"
        << "k_m:"<<k_m<<"\n"
        << "e:"<<e<<"\n"
        << "H:"<<H<<"\n"
        << "L:"<<L<<"\n"
        << "g_m:"<<g_m<<"\n"
        << "numOutScatter:"<<numOutScatter<<"\n"
        << "numInScatter:"<<numInScatter<<"\n"
        << "scale:"<<scale<<"\n"
        << "color:"<<atmosColor[0]<<","<<atmosColor[1]<<","<<atmosColor[2];
   return ststr.str();
}
