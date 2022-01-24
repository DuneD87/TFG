//
// Created by drive on 11/11/21.
//

#include "WaterSphere.h"


WaterSphere::WaterSphere(float planetRadius, float waterRadius, Camera *cam, glm::vec3 position) {
    this->planetRadius = planetRadius;
    this->cam = cam;
    entityShader = Shader("../Shaders/waterShaderVertex.shader","../Shaders/waterShaderFragment.shader");
    this->position = position;
    Cubesphere cubesphere(waterRadius,6,true);
    cubesphere.setupNoise(0,NULL);
    std::vector<Texture> textv;
    Texture text1;
    text1.type = "texture_diffuse";
    text1.id = TextureFromFile("/Planet/dudv.png","../Textures/",false,true);
    text1.path = "/Planet/dudv.png";
    Texture text2;
    text2.type = "texture_diffuse";
    text2.id = TextureFromFile("/Planet/waterTile2.jpg","../Textures/",false,true);
    text2.path = "/planet/reflexion1.jpg";
    textv.push_back(text1);
    textv.push_back(text2);
    entityMesh = new Mesh(cubesphere.vertexList,cubesphere.getIndices(),textv);
    entityMesh->position = position;
}

void WaterSphere::draw(Shader &shader) {
    glEnable(GL_BLEND);
    //glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    entityShader.use();
    glm::mat4 view = this->cam->GetViewMatrix();
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(this->cam->Zoom), (float)setting_scrWidth/setting_scrHeight, setting_near, setting_far);
    glm::mat4 cameraModel(1.0f);
    entityShader.setMat4("view",view);
    entityShader.setMat4("projection",projection);
    entityShader.setMat4("model", cameraModel);
    entityShader.setVec3("viewPos",cam->Position);
    entityShader.setFloat("shininess",12.80f);
    entityShader.setVec3("planetOrigin",position);
    if (moveFactor < 1.0)
        moveFactor += waveSpeed * cam->deltaTime;
    else
        moveFactor = 0.0;
    entityShader.setFloat("waveSpeed",moveFactor);
    entityMesh->Draw(entityShader);
    sun->draw(entityShader);
    glDisable(GL_BLEND);
}

WaterSphere::~WaterSphere() {
    delete entityMesh;
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
        entityMesh->textures.push_back(text);
        isset = true;
    }
}
