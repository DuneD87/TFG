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
    Camera camera;

    Scene(const char * path,unsigned int scrWidth, unsigned int scrHeight, Camera &camera);
    void renderScene();
    void setPostProcess(unsigned int index);


private:
    void renderLoopCamera(Shader shader);
    void setupFrameBuffer();
    GLFWwindow *window;
    std::vector<Model> models;
    std::vector<Light> lights;
    std::vector<Mesh> effects;
    std::vector<Shader> shaders;

    Light spotLight = Light("", glm::vec3(), glm::vec3(), glm::vec3(), 0, 0, 0);
    unsigned int scrWidth,scrHeight,frameBuffer,textColorBuffer,rbo,quadVAO,quadVBO;
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
