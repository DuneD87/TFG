//
// Created by drive on 15/7/21.
//

#include "Entity.h"

void Entity::setModel(const char *path) {
    this->entityModel = Model(path);
}

Model Entity::getModel() {
    return this->entityModel;
}

glm::vec3 Entity::getPosition() {
    return this->_position;
}

glm::vec4 Entity::getRotation() {
    return this->_rotation;
}

glm::vec3 Entity::getScale() {
    return this->_scale;
}

void Entity::setPosition(glm::vec3 position) {
    this->_position = position;
}

void Entity::setRotation(glm::vec4 rotation) {
    this->_rotation = rotation;
}

void Entity::setScale(glm::vec3 scale) {
    this->_scale = scale;
}


Entity::Entity(int id, int type) {
    this->id = id;
    this->type = type;
}

Entity::Entity(int id, int type, const char *modelPath) {
    this->id = id;
    this->type = type;
    this->entityModel = Model(modelPath);
}


Entity::Entity(int id, int type, Model model) {
    this->id = id;
    this->type = type;
    this->entityModel = model;
}

int Entity::getType() {
    return type;
}

void Entity::draw(Shader &shader, bool outlined, int depthMap) {
    //FIND A WAY TO CALL CHILD METHOD
}

