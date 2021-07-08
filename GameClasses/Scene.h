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

    Scene(const char * path,unsigned int frameBuffer,unsigned int scrWidth, unsigned int scrHeight, Camera &camera);
    void renderScene();



private:
    void renderLoopCamera(Shader shader);
    GLFWwindow *window;
    std::vector<Model> models;
    std::vector<Light> lights;
    std::vector<Mesh> effects;
    std::vector<Shader> shaders;

    Light spotLight = Light("", glm::vec3(), glm::vec3(), glm::vec3(), 0, 0, 0);
    unsigned int scrWidth,scrHeight;
};


#endif //OPENGLTEST_SCENE_H
