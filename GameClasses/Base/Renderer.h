//
// Created by drive on 8/7/21.
//

#ifndef OPENGLTEST_RENDERER_H
#define OPENGLTEST_RENDERER_H
#include<iostream>
#include<vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Entities/Light.h"
#include "../../OldStuff/BasicTerrain.h"
#include "../Basic/Model.h"
#include "../Basic/Camera.h"
#include "../Basic/Shader.h"
#include "../Util/XmlParser.h"
#include "../Util/BasicShapeBuilder.h"
#include "EngineSettings.h"

class Renderer {
public:
    int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
    int nPointLights;
    Camera *camera;
    bool enableSpotLight = false;
    Renderer();
    ~Renderer();
    Renderer(unsigned int scrWidth, unsigned int scrHeight, Camera *camera, const char* skyboxPath);
    void addSun(Light* sunLight);
    void preRender(vector<Entity*> worldEnts);
    void renderScene(vector<Entity*> worldEnts,std::vector<std::pair<std::vector<glm::mat4>,PhysicsObject*>>ents);
    void setSunDir(glm::vec3 sunDirection);
    void setPostProcess(unsigned int index);
    void addShader(Shader &shader);
    void removeShader(int shaderId);

private:
    void drawEntities(std::vector<Entity*> worldEnts, glm::mat4 view, glm::mat4 projection, Shader &shader);
    void renderInstanced(std::pair<std::vector<glm::mat4>,PhysicsObject*>ent, Shader &shader);
    void renderShadowMap(vector<Entity*> worldEnts,std::vector<std::pair<std::vector<glm::mat4>,PhysicsObject*>>ents);
    void renderLoopCamera(Shader shader,glm::mat4 view, glm::mat4 projection,bool skybox = false);
    void renderReflexionTexture(vector<Entity*> worldEnts,std::vector<std::pair<std::vector<glm::mat4>,PhysicsObject*>>ents);
    void setupFrameBuffer();
    void setupSkyBox(const char * path);
    unsigned int loadCubemap(vector<std::string> faces);
    GLFWwindow *window;
    std::vector<Mesh> effects;
    std::vector<Shader> shaders;
    glm::vec3 sunPos = glm::vec3(0);
    glm::vec3 sunDir = glm::vec3(0);
    glm::mat4 lightSpaceMatrix;
    PhysicsObject *water;
    Light spotLight = Light("", glm::vec3(), glm::vec3(), glm::vec3(), 0, 0, 0,-1,-1);
    Light* sun;
    unsigned int frameBuffer,textColorBuffer,rbo,quadVAO,quadVBO,skyboxVAO,skyboxVBO,cubemapTexture,
            depthMapFBO,depthMap,reflexionFBO,reflexion,refractionFBO,refraction;
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
