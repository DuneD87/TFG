//
// Created by drive on 15/7/21.
//

#include "World.h"

World::World(const char *scenePath, const char *skyBoxPath, unsigned int scrWidth, unsigned int scrHeight,
             Camera *camera) {
    renderer = new Renderer(scrWidth,scrHeight,camera,skyBoxPath);
    XmlParser parser(scenePath);
    worldEntities = parser._ents;
    treeAmount = 0;
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
    treeModel = new PhysicsObject(-1,0,"../Models/treeLP1.obj");

    for (int i = 0; i < wSeg; i+=4)
    {
        for (int j = 0; j < wSeg; j+=4)
        {
            int index = i*j+j;
            glm::vec3 groundPos = terrain->terrainMesh->vertices[index].Position;
            if (groundPos.y < mPoint - abs(hPoint*0.1) && groundPos.y > mPoint - abs(0.30*lPoint))
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model,groundPos);
                model = glm::rotate(model,0.0f,glm::vec3(1.0f));
                model = glm::scale(model,glm::vec3(1));
                treeAmount++;
                treePos.push_back(model);
            }
        }
    }
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, treeAmount * sizeof(glm::mat4), &treePos[0], GL_STATIC_DRAW);
    for (unsigned int i = 0; i < treeModel->getModel()->meshes.size(); i++)
    {
        unsigned int VAO = treeModel->getModel()->meshes[i]->VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
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
    renderer->renderScene(worldEntities,treePos,treeModel,treeAmount);

}

Renderer* World::getRenderer() {
    return renderer;
}

World::~World() {
    for (auto ent : worldEntities)
        delete ent;
    delete renderer;
}
