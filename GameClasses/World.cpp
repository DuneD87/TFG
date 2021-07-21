//
// Created by drive on 15/7/21.
//

#include "World.h"

World::World(const char *scenePath, const char *skyBoxPath, unsigned int scrWidth, unsigned int scrHeight,
             Camera &camera) {
    renderer = Renderer(scrWidth,scrHeight,camera,skyBoxPath);
    XmlParser parser(scenePath);
    worldEntities = parser._ents;
    renderer.camera = camera;
}

void World::addEntity(Entity *entity) {

}

void World::removeEntity(int id) {

}

void World::setSunPosition(glm::vec3 position) {

}

void World::addLightToWorld(Light &light) {

}

void World::renderWorld() {
    renderer.camera = this->camera;
    renderer.renderScene(worldEntities);
}
