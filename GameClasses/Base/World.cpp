//
// Created by drive on 15/7/21.
//

#include "World.h"
#include "../Entities/BasicWater.h"
#include "../Entities/Planet/Planet.h"
#include "../Util/libs/imgui.h"
#include "../Util/libs/imgui_impl_opengl3.h"

World::World(const char *scenePath, const char *skyBoxPath, unsigned int scrWidth, unsigned int scrHeight,
             Camera *camera) {
    renderer = new Renderer(scrWidth,scrHeight,camera,skyBoxPath);
    parser =  new XmlParser(scenePath,camera);
    worldEntities = parser->_ents;
    nPointLights = parser->nPointLights;
    renderer->nPointLights = nPointLights;
    renderer->camera = camera;
    this->sun = parser->sun;
    sunPitch = sun->getDirection().x;
    sunYaw = sun->getDirection().y;
    //camera->Position = glm::vec3(29562.6,-20292.2,58728);
}

void World::renderWorld() {

    renderer->renderScene(worldEntities,worldDeco);
    ImGui::Begin("World Settings",NULL,ImGuiWindowFlags_MenuBar);
    ImGui::SetWindowFontScale(2);
    float xzLen = cos(glm::radians(sunPitch));
    float x = xzLen * cos(glm::radians(sunYaw));
    float y = sin(glm::radians(sunPitch));
    float z = xzLen * sin(glm::radians(-sunYaw));
    glm::vec3 direction(x,y,z);
    sun->setDirection(glm::vec2(sunPitch,sunYaw));
    renderer->setSunDir(direction);
    ImGui::SliderFloat("Sun yaw",&sunYaw,-180,180);
    ImGui::SliderFloat("Sun pitch",&sunPitch,-90,90);
    bool saveWorld = ImGui::Button("Save");
    if (saveWorld) parser->saveWorld();
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Renderer* World::getRenderer() {
    return renderer;
}

World::~World() {
    for (auto ent : worldEntities)
        delete ent;
    delete renderer;
    delete parser;
}
