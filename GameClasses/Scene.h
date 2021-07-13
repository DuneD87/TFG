//
// Created by drive on 8/7/21.
//

#ifndef OPENGLTEST_SCENE_H
#define OPENGLTEST_SCENE_H
#include<iostream>
#include<vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Light.h"
#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include "XmlParser.h"
#include "BasicShapeBuilder.h"

class Scene {
public:
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    Camera camera;
    bool enableSpotLight = false;
    Scene(const char * path,unsigned int scrWidth, unsigned int scrHeight, Camera &camera, const char* skyboxPath);
    void renderScene();
    void setPostProcess(unsigned int index);


private:
    void renderShadowMap();
    void renderLoopCamera(Shader shader,bool skybox = false);
    void setupFrameBuffer();
    void setupSkyBox(const char * path);
    unsigned int loadCubemap(vector<std::string> faces);
    GLFWwindow *window;
    std::vector<Model> models;
    std::vector<Light> lights;
    std::vector<Mesh> effects;
    std::vector<Shader> shaders;
    glm::vec3 lightPos = glm::vec3(2.0f, 0.0f, 4.0f);
    glm::mat4 lightSpaceMatrix;

    Light spotLight = Light("", glm::vec3(), glm::vec3(), glm::vec3(), 0, 0, 0);
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


#endif //OPENGLTEST_SCENE_H
