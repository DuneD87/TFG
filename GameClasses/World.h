//
// Created by drive on 15/7/21.
//

#ifndef OPENGLTEST_WORLD_H
#define OPENGLTEST_WORLD_H

#include "Renderer.h"
#include "Entities/Entity.h"

class World {
public:
    World(const char * scenePath, const char * skyBoxPath, unsigned int scrWidth, unsigned int scrHeight, Camera &camera);

    void addEntity(Entity entity);
    void removeEntity(int id);

    void setSunPosition(glm::vec3 position);
private:
    void addLightToWorld(Light &light);

    vector<Entity> worldEntities;

};


#endif //OPENGLTEST_WORLD_H
