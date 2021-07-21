//
// Created by drive on 16/7/21.
//

#ifndef OPENGLTEST_PHYSICSOBJECT_H
#define OPENGLTEST_PHYSICSOBJECT_H

#include "Entity.h"
class PhysicsObject : public Entity {
public:
    PhysicsObject(int entId,int type,const char * path);
    PhysicsObject(int entId,int type,Model &model);

    void draw(Shader &shader, bool outlined, int depthMap) override;
};


#endif //OPENGLTEST_PHYSICSOBJECT_H
