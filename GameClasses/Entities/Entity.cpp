//
// Created by drive on 15/7/21.
//

#include "Entity.h"

Entity::Entity(const char *path) {
    entityModel = Model(path);
    type = 0;//Deco
}

Entity::Entity(int type, const char * path) {

}

Model Entity::getModel() {
    return Model();
}

glm::vec3 Entity::getPosition() {
    return glm::vec3();
}

glm::vec4 Entity::getRotation() {
    return glm::vec4();
}

glm::vec3 Entity::getScale() {
    return glm::vec3();
}

void Entity::setPosition(glm::vec3 position) {

}

void Entity::setRotation(glm::vec4 rotation) {

}

void Entity::setScale(glm::vec3 scale) {

}
