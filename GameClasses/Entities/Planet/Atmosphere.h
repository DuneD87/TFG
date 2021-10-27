//
// Created by drive on 5/8/21.
//

#ifndef OPENGLTEST_ATMOSPHERE_H
#define OPENGLTEST_ATMOSPHERE_H

#include "../Entity.h"
#include "../../Basic/Camera.h"
#include "Cubesphere.h"

class Atmosphere : public Entity {
public:
    Atmosphere(float planetRadius, float atmosRadius, Camera *cam,glm::vec3 position);
    Atmosphere(float planetRadius, float atmosRadius, Camera *cam, float kR, float kM, float e, float h, float l,
               float atmosColor[3], float gM, float numOutScatter, float numInScatter, float scale,glm::vec3 position);
    ~Atmosphere();
    void draw(Shader &shader, bool outlined = false, int depthMap = -1);
    void setSunDir(glm::vec3 sunDir);
    std::string toString();
private:
    void renderGui();
    Model * skyDome;
    float planetRadius, atmosRadius;
    Camera *cam;
private:
    Shader atmosShader;
    Shader atmosShaderIN;
    float viewDistance = 1000000.0f;
    float k_r = 0.166;
    float k_m = 0.0025;
    float e = 14.3;
    float H = 4.0f;
    float L = 1.0f;
    float atmosColor[3] = {0.1,0.2,0.8};
    glm::vec3 sunDir = glm::vec3(-2,-4,-2);
    float g_m = -0.85;
    float numOutScatter = 10.0f;
    float numInScatter = 10.0f;
    float scale = 1;
};


#endif //OPENGLTEST_ATMOSPHERE_H
