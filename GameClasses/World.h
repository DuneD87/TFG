//
// Created by drive on 15/7/21.
//

#ifndef OPENGLTEST_WORLD_H
#define OPENGLTEST_WORLD_H

#include "Renderer.h"
#include "Entities/BasicTerrain.h"

class World {
public:
    Camera camera;
    int nPointLights;

    ~World();

    World(const char * scenePath, const char * skyBoxPath, unsigned int scrWidth, unsigned int scrHeight, Camera *camera);

    void addEntity(Entity *entity);
    Renderer* getRenderer();
    void removeEntity(int id);
    void renderWorld();
    void setSunPosition(glm::vec3 position);
private:
    void addLightToWorld(Light &light);
    BasicTerrain *terrain;
    vector<Entity*> worldEntities;
    Renderer *renderer;
};


#endif //OPENGLTEST_WORLD_H
