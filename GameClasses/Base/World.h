//
// Created by drive on 15/7/21.
//

#ifndef OPENGLTEST_WORLD_H
#define OPENGLTEST_WORLD_H

#include "Renderer.h"
#include "../Util/Math.h"
#include "../../OldStuff/BasicTerrain.h"
#include "../Base/EngineSettings.h"
class World {
public:
    Camera camera;
    int nPointLights;
    int waterLevel = -50;
    ~World();

    World(const char * scenePath, const char * skyBoxPath, unsigned int scrWidth, unsigned int scrHeight, Camera *camera);


    Renderer* getRenderer();
    void renderWorld();
private:
    void setupInstanceObjects(int wSeg, int divider);
    void makeWaterLevel();
    BasicTerrain *terrain;
    Mesh waterPlane;
    PhysicsObject * water;
    vector<Entity*> worldEntities;
    Light *sun;
    float sunYaw,sunPitch;
    vector<std::pair<std::vector<glm::mat4>,PhysicsObject*>> worldDeco;
    Renderer *renderer;
    XmlParser *parser;
};


#endif //OPENGLTEST_WORLD_H
