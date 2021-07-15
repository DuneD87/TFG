//
// Created by drive on 15/7/21.
//

#ifndef OPENGLTEST_ENTITY_H
#define OPENGLTEST_ENTITY_H
#include <iostream>
#include "../Basic/Model.h"
#include <reactphysics3d/reactphysics3d.h>

class Entity {
public:
    Entity(const char * path);
    Entity(int type, const char * path = "");

    Model getModel();

    virtual glm::vec3 getPosition();
    virtual glm::vec4 getRotation();
    virtual glm::vec3 getScale();

    virtual void setPosition(glm::vec3 position);
    void setRotation(glm::vec4 rotation);
    void setScale(glm::vec3 scale);
private:
    Model entityModel;
    reactphysics3d::RigidBody* body;
    int id;
    /*
    * Types:
     * 0-> LIGHT
     * 1-> DECORATION
     * 2-> EFFECTS
     * 3-> TERRAIN
    */
    int type;

};


#endif //OPENGLTEST_ENTITY_H
