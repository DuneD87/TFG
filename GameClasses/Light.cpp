//
// Created by drive on 2/7/21.
//

#include "Light.h"

Light::Light(Type type, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant,
float linear, float quadratic) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
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

Type Light::getType() {
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


