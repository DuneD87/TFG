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
    int id;

    Entity(int id, int type);
    Entity(int id, int type, Model model);
    Entity(int id, int type, const char * modelPath);

    Model getModel();
    void setModel(const char * path);
    virtual glm::vec3 getPosition();
    virtual glm::vec4 getRotation();
    virtual glm::vec3 getScale();
    virtual int getType();

    virtual void draw(Shader &shader, bool outlined = false, int depthMap = -1);

    virtual void setPosition(glm::vec3 position);
    virtual void setRotation(glm::vec4 rotation);
    virtual void setScale(glm::vec3 scale);
protected:
    glm::vec3 _position;
    glm::vec4 _rotation;
    glm::vec3 _scale;
    Model entityModel;
    /*
   * Types:
    * 0-> LIGHT
    * 1-> PHYSICSOBJECT
    * 2-> EFFECTS
    * 3-> TERRAIN
   */
    int type;
private:


};


#endif //OPENGLTEST_ENTITY_H
