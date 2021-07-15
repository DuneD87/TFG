//
// Created by drive on 15/7/21.
//

#include "World.h"

World::World(const char *scenePath, const char *skyBoxPath, unsigned int scrWidth, unsigned int scrHeight,
             Camera &camera) {
    Light light("dirLight",glm::vec3(0,0,0),glm::vec3(0,0,0),glm::vec3(0,0,0) , glm::vec3(0,0,0));
    worldEntities.push_back(light);
}

void World::addEntity(Entity entity) {

}

void World::removeEntity(int id) {

}

void World::setSunPosition(glm::vec3 position) {

}

void World::addLightToWorld(Light &light) {

}
