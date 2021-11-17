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
    waterMesh = new Mesh(cubesphere.vertexList,cubesphere.getIndices(),"/Planet/dudv.png","");
    waterMesh->position = position;
}

void WaterSphere::draw(Shader &shader, bool outlined, int depthMap) {
    glEnable(GL_BLEND);
    //glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    waterShader.use();
    glm::mat4 view = this->cam->GetViewMatrix();
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(this->cam->Zoom), (float)setting_scrWidth/setting_scrHeight, setting_near, setting_far);
    glm::mat4 cameraModel(1.0f);
    waterShader.setMat4("view",view);
    waterShader.setMat4("projection",projection);
    waterShader.setMat4("model", cameraModel);
    waterShader.setVec3("viewPos",cam->Position);
    waterShader.setFloat("material.shininess",12.80f);
    waterShader.setVec3("planetOrigin",position);

    if (moveFactor < 1.0)
        moveFactor += waveSpeed * cam->deltaTime;
    else
        moveFactor = 0.0;
    std::cout<<moveFactor<<std::endl;
    waterShader.setFloat("waveSpeed",moveFactor);
    waterMesh->Draw(waterShader,outlined,depthMap,false,false);
    sun->draw(waterShader,outlined,depthMap);
    oldClock = clock();
    glDisable(GL_BLEND);
}

WaterSphere::~WaterSphere() {
    delete waterMesh;
}

std::string WaterSphere::toString() {
    return std::string();
}

void WaterSphere::setSun(Light *sun) {
    this->sun = sun;
}

void WaterSphere::setWaterTexture(unsigned int waterText) {
    if (!isset)
    {
        Texture text;
        text.path ="";
        text.type ="texture_diffuse";
        text.id = waterText;
        waterMesh->textures.push_back(text);
        isset = true;
    }

}
