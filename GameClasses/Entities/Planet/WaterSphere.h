//
// Created by drive on 11/11/21.
//

#ifndef OPENGLTEST_WATERSPHERE_H
#define OPENGLTEST_WATERSPHERE_H

#include "../../Util/Cubesphere.h"
#include "../Entity.h"
#include "../../Basic/Camera.h"
#include "../../Base/EngineSettings.h"
#include "../Light.h"

class WaterSphere : public Entity {
public:
    WaterSphere(float planetRadius, float waterRadius, Camera *cam, glm::vec3 position);
    ~WaterSphere();
    void draw(Shader &shader, bool outlined = false, int depthMap = -1);
    void setSun(Light * sun);
    std::string toString();
private:
    int oldClock;
    Shader waterShader;
    float planetRadius,waterRadius;
    Mesh* waterMesh;
    Camera* cam;
    glm::vec3 position;
    Light* sun;
};


#endif //OPENGLTEST_WATERSPHERE_H
