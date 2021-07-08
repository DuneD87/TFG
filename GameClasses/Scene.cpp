//
// Created by drive on 8/7/21.
//

#include "Scene.h"


Scene::Scene(const char * path,unsigned int frameBuffer,unsigned int scrWidth, unsigned int scrHeight, Camera &camera) {
    this->scrWidth = scrWidth;
    this->scrHeight = scrHeight;
    this->camera = camera;
    spotLight =  Light("spotLight",glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(1.0f, 1.0f, 1.0f),
                                 glm::vec3(1.0f, 1.0f, 1.0f),1.0,0.09,0.032);
    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightShader("../Shaders/lightingShader.vs", "../Shaders/lightingShader.fs");
    Shader spriteShader("../Shaders/lightingShader.vs","../Shaders/alphaTextureTest.fs");
    Shader outlineShader = Shader("../Shaders/lightingShader.vs", "../Shaders/singleColorShader.fs");
    shaders.push_back(lightShader);
    shaders.push_back(spriteShader);
    shaders.push_back(outlineShader);
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    XmlParser parser("../Scenes/Scene1.xml");
    models = parser._models;
    lights = parser._lights;
    effects = parser._effects;
}

void Scene::renderScene() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // set uniforms
    shaders[2].use();
    renderLoopCamera(shaders[2]);
    shaders[1].use();
    renderLoopCamera(shaders[1]);
    shaders[0].use();
    renderLoopCamera(shaders[0]);
    shaders[0].setFloat("material.shininess", 64.0f);
    shaders[0].setVec3("viewPos",camera.Position);
    shaders[0].addLights(lights);

    //Draw
    std::vector<int> selectedeItems;
    for (int i = 0; i < models.size();i++)
    {
        models[i].Draw(shaders[0],false);

    }
    shaders[1].use();
    for (int i = 0; i < effects.size();i++)
    {
        //Render should be a function, handle semi-transparent objects sorting them to draw in order ( pre-render function? )
        effects[i].Draw(shaders[1],false);
    }
    //drawables[1].outlineObject(outlineShader,glm::vec3(1.1));
    for (int i = 0; i < selectedeItems.size();i++)
        models[selectedeItems[i]].outlineObject(shaders[2],glm::vec3(1.1));

}

void Scene::renderLoopCamera(Shader shader) {
    //view
    glm::mat4 view;
    view = this->camera.GetViewMatrix();
    shader.setMat4("view",view);

    //projection
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(this->camera.Zoom), (float)scrWidth/scrHeight, 0.1f, 100.0f);
    shader.setMat4("projection",projection);
    glm::mat4 cameraModel(1.0f);
    shader.setMat4("model", cameraModel);
}
