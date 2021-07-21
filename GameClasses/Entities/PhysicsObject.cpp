//
// Created by drive on 16/7/21.
//

#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(int entId,int type, const char *path){
    this->id = id;
    this->type = 1;
    this->entityModel = Model(path);
}

PhysicsObject::PhysicsObject(int entId,int type, Model &model){
    this->id = id;
    this->type = 1;
    this->entityModel = model;
}

void PhysicsObject::draw(Shader &shader, bool outlined, int depthMap) {
    this->entityModel.setPosition(_position);
    this->entityModel.setRotation(_rotation.w,glm::vec3(_rotation));
    this->entityModel.setScale(_scale);
    this->entityModel.Draw(shader,outlined,depthMap);
}
