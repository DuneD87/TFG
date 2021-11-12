//
// Created by drive on 11/11/21.
//

#include "WaterSphere.h"


WaterSphere::WaterSphere(float planetRadius, float waterRadius, Camera *cam, glm::vec3 position) {
    this->planetRadius = planetRadius;
    this->waterRadius = waterRadius;
    this->cam = cam;
    waterShader = Shader("../Shaders/waterShaderVertex.shader","../Shaders/waterShaderFragment.shader");
    this->position = position;
    Cubesphere cubesphere(waterRadius,6,true);
    cubesphere.setupNoise(0,NULL);
    waterMesh = new Mesh(cubesphere.vertexList,cubesphere.getIndices(),"/Planet/reflection.jpg","");
    waterMesh->position = position;
}

void WaterSphere::draw(Shader &shader, bool outlined, int depthMap) {
    //glDisable(GL_DEPTH_TEST);
    waterShader.use();
    glm::mat4 view = this->cam->GetViewMatrix();
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(this->cam->Zoom), (float)setting_scrWidth/setting_scrHeight, setting_near, setting_far);
    glm::mat4 cameraModel(1.0f);
    waterShader.setMat4("view",view);
    waterShader.setMat4("projection",projection);
    waterShader.setMat4("model", cameraModel);
    waterMesh->Draw(waterShader,outlined,depthMap,false,false);
    //glEnable(GL_DEPTH_TEST);
}

void WaterSphere::setSunDir(glm::vec3 sunDir) {

}

WaterSphere::~WaterSphere() {
    delete waterMesh;
}

std::string WaterSphere::toString() {
    return std::string();
}
