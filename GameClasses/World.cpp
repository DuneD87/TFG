//
// Created by drive on 15/7/21.
//

#include "World.h"

World::World(const char *scenePath, const char *skyBoxPath, unsigned int scrWidth, unsigned int scrHeight,
             Camera *camera) {
    renderer = Renderer(scrWidth,scrHeight,camera,skyBoxPath);
    XmlParser parser(scenePath);
    worldEntities = parser._ents;

    nPointLights = parser.nPointLights;
    renderer.nPointLights = nPointLights;
    renderer.camera = camera;
    std::vector<Texture> text;
    terrain = new BasicTerrain(2000,2000,256,256,glm::vec3(0),text);
    worldEntities.push_back(terrain);
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
    renderer.renderScene(worldEntities);
}

Renderer* World::getRenderer() {
    return &renderer;
}
