//
// Created by drive on 15/7/21.
//

#ifndef OPENGLTEST_WORLD_H
#define OPENGLTEST_WORLD_H

#include "Renderer.h"
#include "../Util/Math.h"
#include "../Base/EngineSettings.h"
class World {
public:
    Camera* camera;
    int nPointLights;
    int waterLevel = -50;
    ~World();

    World(const char * scenePath, const char * skyBoxPath, unsigned int scrWidth, unsigned int scrHeight, Camera *camera);


    Renderer* getRenderer();
    void renderWorld(bool wireframe);
private:

    vector<Entity*> worldEntities;
    Light *sun;
    float sunYaw,sunPitch;
    Renderer *renderer;
    XmlParser *parser;
};


#endif //OPENGLTEST_WORLD_H
