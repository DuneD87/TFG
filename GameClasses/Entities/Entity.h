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
    Model getModel();
    virtual void setModel(const char * path);
    virtual glm::vec3 getPosition();
    virtual glm::vec4 getRotation();
    virtual glm::vec3 getScale();

    virtual void draw(Shader &shader);

    virtual void setPosition(glm::vec3 position);
    virtual void setRotation(glm::vec4 rotation);
    virtual void setScale(glm::vec3 scale);
private:
    Model entityModel;
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
