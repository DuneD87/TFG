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
    std::vector<Texture> text;
    waterMesh = new Mesh(cubesphere.vertexList,cubesphere.getIndices(),"/Planet/reflection.jpg","");
    waterMesh->position = position;
}

void WaterSphere::draw(Shader &shader, bool outlined, int depthMap) {
    //glDisable(GL_DEPTH_TEST);
    waterShader.use();
    float far =  10000000.0f;
    float near = 0.1f;
    glm::mat4 view = this->cam->GetViewMatrix();
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(this->cam->Zoom), (float)1920/1080, near, far);
    glm::mat4 cameraModel(1.0f);
    waterShader.setMat4("view",view);
    waterShader.setMat4("projection",projection);
    waterShader.setMat4("model", cameraModel);
    waterShader.setFloat("near",near);
    waterShader.setFloat("far",far);
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
