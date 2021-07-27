//
// Created by drive on 15/7/21.
//

#include "World.h"

World::World(const char *scenePath, const char *skyBoxPath, unsigned int scrWidth, unsigned int scrHeight,
             Camera *camera) {
    renderer = new Renderer(scrWidth,scrHeight,camera,skyBoxPath);
    XmlParser parser(scenePath);
    worldEntities = parser._ents;

    nPointLights = parser.nPointLights;
    renderer->nPointLights = nPointLights;
    renderer->camera = camera;
    std::vector<Texture> text;
    const char *textures[] = {"grassy2.png","grassFlowers.png","mud.png","seamless_rock2.png"};
    int wSeg= 1024;
    terrain = new BasicTerrain(8000, 8000, wSeg, wSeg, glm::vec3(0), text, textures,4);
    float hPoint = terrain->getHighestPoint();
    float lPoint = terrain->getLowestPoint();
    float mPoint = lPoint-((lPoint-hPoint)/2);
    for (int i = 0; i < wSeg; i+=8)
    {
        for (int j = 0; j < wSeg; j+=8)
        {
            int index = i*j+j;
            glm::vec3 groundPos = terrain->terrainMesh->vertices[index].Position;
            if (groundPos.y < mPoint - 40 && groundPos.y > mPoint - 100)
            {
                PhysicsObject *tree = new PhysicsObject(-1,0,"../Models/treeLP1.obj");
                tree->setPosition(groundPos);
                worldEntities.push_back(tree);
            }
        }
    }
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
    renderer->renderScene(worldEntities);
}

Renderer* World::getRenderer() {
    return renderer;
}

World::~World() {
    for (auto ent : worldEntities)
        delete ent;
    delete renderer;
}
