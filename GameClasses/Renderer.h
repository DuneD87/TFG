//
// Created by drive on 8/7/21.
//

#ifndef OPENGLTEST_RENDERER_H
#define OPENGLTEST_RENDERER_H
#include<iostream>
#include<vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Entities/Light.h"
#include "Basic/Model.h"
#include "Basic/Camera.h"
#include "Basic/Shader.h"
#include "XmlParser.h"
#include "BasicShapeBuilder.h"

class Renderer {
public:
    unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    Camera camera;
    bool enableSpotLight = false;
    Renderer();
    Renderer(unsigned int scrWidth, unsigned int scrHeight, Camera &camera, const char* skyboxPath);
    void renderScene(vector<Entity> worldEnts);
    void setPostProcess(unsigned int index);
    void addShader(Shader &shader);
    void removeShader(int shaderId);

private:
    void renderShadowMap(vector<Entity> worldEnts);
    void renderLoopCamera(Shader shader,bool skybox = false);
    void setupFrameBuffer();
    void setupSkyBox(const char * path);
    unsigned int loadCubemap(vector<std::string> faces);
    GLFWwindow *window;
    std::vector<Model> models;
    std::vector<Light> lights;
    std::vector<Mesh> effects;
    std::vector<Shader> shaders;
    glm::vec3 sunPos = glm::vec3(2.0f, -1.0f, 2.0f);
    glm::mat4 lightSpaceMatrix;

    Light spotLight = Light("", glm::vec3(), glm::vec3(), glm::vec3(), 0, 0, 0,-1);
    unsigned int scrWidth,scrHeight,frameBuffer,textColorBuffer,rbo,quadVAO,quadVBO,skyboxVAO,skyboxVBO,cubemapTexture,
            depthMapFBO,depthMap;
    std::string postProcessPath[6] = {
            "../Shaders/PostProcess/blurShader.fs",
            "../Shaders/PostProcess/edgeShader.fs",
            "../Shaders/PostProcess/grayScaleShader.fs",
            "../Shaders/PostProcess/inverseShader.fs",
            "../Shaders/PostProcess/sharpenShader.fs",
            "../Shaders/PostProcess/screenShader.fs"
    };
};


#endif //OPENGLTEST_RENDERER_H
