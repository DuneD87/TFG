//
// Created by drive on 2/7/21.
//

#include "Light.h"

Light::Light(std::string lightType, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant,
             float linear, float quadratic, int entId,int lightIndex) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
    this->subType = lightType;
    this->id = entId;
    this->type = 2;
    this->lightIndex = lightIndex;
}

Light::Light(std::string lightType, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec2 orientation,
             int entId){
    this->subType = lightType;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->orientation = orientation;
    this->id = entId;
    this->type = 2;
}

void Light::setAmbient(glm::vec3 ambient) {
    this->ambient = ambient;
}

void Light::setDiffuse(glm::vec3 diffuse) {
    this->diffuse = diffuse;
}

void Light::setSpecular(glm::vec3 specular) {
    this->specular = specular;
}

std::string Light::getSubType() {
    return this->subType;
}

void Light::setDirection(glm::vec2 orientation) {
    this->orientation = orientation;
}

void Light::setCutOff(float cutOff) {
 this->cutOff = cutOff;
}

void Light::setOuterCutOff(float outerCutOff) {
    this->outerCutOff = outerCutOff;
}

float Light::getQuadratic() {
    return this->quadratic;
}

float Light::getLinear() {
    return this->linear;
}

float Light::getConstant() {
    return this->constant;
}

glm::vec3 Light::getSpecular() {
    return this->specular;
}

glm::vec3 Light::getDiffuse() {
    return this->diffuse;
}

glm::vec3 Light::getAmbient() {
    return this->ambient;
}

glm::vec2 Light::getDirection() {
    return this->orientation;
}

void Light::draw(Shader &shader,bool outlined, int depthMap) {
    if (this->getSubType() == "pointLight") {
        shader.setVec3("pointLights["+std::to_string(this->lightIndex)+"].position", this->getPosition());
        shader.setVec3("pointLights["+std::to_string(this->lightIndex)+"].ambient",this->getAmbient());
        shader.setVec3("pointLights["+std::to_string(this->lightIndex)+"].diffuse", this->getDiffuse());
        shader.setVec3("pointLights["+std::to_string(this->lightIndex)+"].specular",this->getSpecular());
        shader.setFloat("pointLights["+std::to_string(this->lightIndex)+"].constant", this->getConstant());
        shader.setFloat("pointLights["+std::to_string(this->lightIndex)+"].linear", this->getLinear());
        shader.setFloat("pointLights["+std::to_string(this->lightIndex)+"].quadratic", this->getQuadratic());
    }
    else if (this->getSubType() == "dirLight") {
        float xzLen = cos(glm::radians(orientation[0]));
        float x = xzLen * cos(glm::radians(orientation[1]));
        float y = sin(glm::radians(orientation[0]));
        float z = xzLen * sin(glm::radians(-orientation[1]));
        glm::vec3 dir(x,y,z);
        shader.setVec3("dirLight.direction", dir);
        shader.setVec3("dirLight.ambient",this->getAmbient());
        shader.setVec3("dirLight.diffuse", this->getDiffuse());
        shader.setVec3("dirLight.specular",this->getSpecular());
    }
}

Light::~Light() {
    //check light type and set values to 0 in the shaders uniforms so the light doesnt stick
}

std::string Light::toString() {
    return std::string("Orientation:"+ std::to_string(orientation[0]) + ","
                                + std::to_string(orientation[1]));
}

glm::vec3 Light::getDirVector() {
    float xzLen = cos(glm::radians(orientation[0]));
    float x = xzLen * cos(glm::radians(orientation[1]));
    float y = sin(glm::radians(orientation[0]));
    float z = xzLen * sin(glm::radians(-orientation[1]));
    glm::vec3 direction(x,y,z);
    return direction;
}

void Light::draw() {
    if (this->getSubType() == "pointLight") {
        entityShader.setVec3("pointLights["+std::to_string(this->lightIndex)+"].position", this->getPosition());
        entityShader.setVec3("pointLights["+std::to_string(this->lightIndex)+"].ambient",this->getAmbient());
        entityShader.setVec3("pointLights["+std::to_string(this->lightIndex)+"].diffuse", this->getDiffuse());
        entityShader.setVec3("pointLights["+std::to_string(this->lightIndex)+"].specular",this->getSpecular());
        entityShader.setFloat("pointLights["+std::to_string(this->lightIndex)+"].constant", this->getConstant());
        entityShader.setFloat("pointLights["+std::to_string(this->lightIndex)+"].linear", this->getLinear());
        entityShader.setFloat("pointLights["+std::to_string(this->lightIndex)+"].quadratic", this->getQuadratic());
    }
    else if (this->getSubType() == "dirLight") {
        float xzLen = cos(glm::radians(orientation[0]));
        float x = xzLen * cos(glm::radians(orientation[1]));
        float y = sin(glm::radians(orientation[0]));
        float z = xzLen * sin(glm::radians(-orientation[1]));
        glm::vec3 dir(x,y,z);
        entityShader.setVec3("dirLight.direction", dir);
        entityShader.setVec3("dirLight.ambient",this->getAmbient());
        entityShader.setVec3("dirLight.diffuse", this->getDiffuse());
        entityShader.setVec3("dirLight.specular",this->getSpecular());
    }
}



