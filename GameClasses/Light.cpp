//
// Created by drive on 2/7/21.
//

#include "Light.h"

Light::Light(std::string lightType, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant,
             float linear, float quadratic) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
    this->type = lightType;
}

void Light::setPosition(glm::vec3 position) {
    this->position = position;
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

std::string Light::getType() {
    return this->type;
}

void Light::setDirection(glm::vec3 direction) {
    this->direction = direction;
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

glm::vec3 Light::getDirection() {
    return this->direction;
}

glm::vec3 Light::getPosition() {
    return this->position;
}

std::string Light::toString() {
    printf("Light found with type %s and:\n\tConstant: %f\n\tLinear: %f\n\tQuadratic: %f\n" ,
           type.c_str(),
           constant,
           linear,
           quadratic
    );
    printf("\tPosition:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n",position.x,position.y,position.z);
    printf("\tAmbient:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n",ambient.x,ambient.y,ambient.z);
    printf("\tDiffuse:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n",diffuse.x,diffuse.y,diffuse.z);
    printf("\tSpecular:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n",specular.x,specular.y,specular.z);
}


