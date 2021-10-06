//
// Created by drive on 15/7/21.
//

#include "World.h"
#include "Entities/BasicWater.h"
#include "Entities/Planet/Planet.h"

World::World(const char *scenePath, const char *skyBoxPath, unsigned int scrWidth, unsigned int scrHeight,
             Camera *camera) {
    renderer = new Renderer(scrWidth,scrHeight,camera,skyBoxPath);
    XmlParser parser(scenePath);
    worldEntities = parser._ents;
    nPointLights = parser.nPointLights;
    renderer->nPointLights = nPointLights;
    renderer->camera = camera;

    std::vector<std::string> diffuse = {
            //DiffuseMap
            "Planet/sandy-rocks1-albedo-1024.png",
            "Planet/sandyground-albedo-1024.png",
            "Planet/rough-wet-cobble-albedo-1024.png",
            "Planet/grass1-albedo3-1024.png",
            "Planet/dirt_01_diffuse-1024.png",
            "Planet/worn-bumpy-rock-albedo-1024.png",
            "Planet/rock-snow-ice-albedo-1024.png",
            "Planet/snow-packed-albedo-1024.png",
            //NormalMap
            "Planet/sandy-rocks1-normal-1024.jpg",
            "Planet/sandyground-normal-1024.jpg",
            "Planet/rough-wet-cobble-normal-1024.jpg",
            "Planet/grass1-normal-1024.jpg",
            "Planet/dirt_01_normal-1024.jpg",
            "Planet/worn-bumpy-rock-normal-1024.jpg",
            "Planet/rock-snow-ice-normal-1024.jpg",
            "Planet/snow-packed-normal-1024.jpg",

    };
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    Planet *planet = new Planet(40000,8,glm::vec3(0),camera,diffuse);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
    camera->Position = glm::vec3(29562.6,-20292.2,58728);
    worldEntities.push_back(planet);
}

void World::renderWorld() {
    renderer->renderScene(worldEntities,worldDeco);
}

Renderer* World::getRenderer() {
    return renderer;
}

World::~World() {
    for (auto ent : worldEntities)
        delete ent;
    delete renderer;
}

void World::setupInstanceObjects(int wSeg, int divider) {
    float hPoint = terrain->getHighestPoint();
    float lPoint = terrain->getLowestPoint();
    float mPoint = lPoint-((lPoint-hPoint)/2);
    vector<glm::mat4> tree1Model;
    vector<glm::mat4> tree2Model;
    vector<glm::mat4> grass1Model;
    vector<glm::mat4> grass2Model;
    vector<glm::mat4> stone1Model;
    vector<glm::mat4> stone2Model;
    vector<glm::mat4> stone3Model;
    vector<glm::mat4> stone4Model;
    vector<glm::mat4> bark1Model;
    vector<glm::mat4> bark2Model;
    vector<glm::mat4> shroom1Model;
    vector<glm::mat4> shroom2Model;
    vector<glm::mat4> bush1Model;
    vector<glm::mat4> bush2Model;
    srand(time(NULL));
    int totalItems = 0;
    for (int i = 0; i < wSeg; i+=divider)
    {
        for (int j = 0; j < wSeg; j+=divider)
        {
            int index = i*j+j;

            glm::vec3 groundPos = terrain->terrainMesh->vertices[index].Position;
            glm::vec3 normal = terrain->terrainMesh->vertices[index].Normal;
            //trees
            if (groundPos.y < mPoint - abs(hPoint*0.1) && groundPos.y > mPoint - abs(0.30*lPoint) && normal.y > 0.95 && i % 4 == 0)
            {
                int randN = rand()%2;
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model,glm::vec3(groundPos.x,std::floor(groundPos.y),groundPos.z));
                model = glm::rotate(model,0.0f,glm::vec3(1.0f));
                float scaleFactorX = randomFloat(1.0,2.0);
                float scaleFactorY = randomFloat(1.0,2.0);
                float scaleFactorZ = randomFloat(1.0,2.0);

                model = glm::scale(model,glm::vec3(scaleFactorX,scaleFactorY,scaleFactorZ));
                totalItems++;
                if (randN == 0)
                {

                    tree1Model.push_back(model);
                } else
                {

                    tree2Model.push_back(model);
                }
            }
            //grass

            if (groundPos.y < hPoint - abs(0.7*hPoint) && groundPos.y > lPoint + abs(0.7 * lPoint) && normal.y > 0.85)
            {
                int randN = rand()%12;
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model,glm::vec3(groundPos.x,std::floor(groundPos.y),groundPos.z));
                model = glm::rotate(model,0.0f,glm::vec3(1.0f));
                float scaleFactorX = randomFloat(1.0,2.0);
                float scaleFactorY = randomFloat(1.0,2.0);
                float scaleFactorZ = randomFloat(1.0,2.0);

                model = glm::scale(model,glm::vec3(scaleFactorX,scaleFactorY,scaleFactorZ));
                switch (randN)
                {
                    case 0:
                        grass1Model.push_back(model);
                        break;
                    case 1:
                        grass2Model.push_back(model);
                        break;
                    case 2:
                        stone1Model.push_back(model);
                        break;
                    case 3:
                        stone2Model.push_back(model);
                        break;
                    case 4:
                        stone3Model.push_back(model);
                        break;
                    case 5:
                        stone4Model.push_back(model);
                        break;
                    case 6:
                        bark1Model.push_back(model);
                        break;
                    case 7:
                        bark2Model.push_back(model);
                        break;
                    case 8:
                        shroom1Model.push_back(model);
                        break;
                    case 9:
                        shroom2Model.push_back(model);
                        break;
                    case 10:
                        bush1Model.push_back(model);
                        break;
                    case 12:
                        bush2Model.push_back(model);
                        break;
                }
                if (randN == 0)
                    grass1Model.push_back(model);
                else
                    grass2Model.push_back(model);
            }
        }
    }
    PhysicsObject * tree1 = new PhysicsObject(-1,0,"../Models/treeLP1.obj");
    PhysicsObject * tree2 = new PhysicsObject(-1,0,"../Models/treeLP2.obj");
    PhysicsObject * grass1 = new PhysicsObject(-1,0,"../Models/grass1.obj");
    PhysicsObject * grass2 = new PhysicsObject(-1,0,"../Models/grass2.obj");
    PhysicsObject * stone1 = new PhysicsObject(-1,0,"../Models/rock1.obj");
    PhysicsObject * stone2 = new PhysicsObject(-1,0,"../Models/rock2.obj");
    PhysicsObject * stone3 = new PhysicsObject(-1,0,"../Models/rock3.obj");
    PhysicsObject * stone4 = new PhysicsObject(-1,0,"../Models/rock4.obj");
    PhysicsObject * bark1 = new PhysicsObject(-1,0,"../Models/bark1.obj");
    PhysicsObject * bark2 = new PhysicsObject(-1,0,"../Models/bark2.obj");
    PhysicsObject * shroom1 = new PhysicsObject(-1,0,"../Models/shroom1.obj");
    PhysicsObject * shroom2 = new PhysicsObject(-1,0,"../Models/shroom2.obj");
    PhysicsObject * bush1 = new PhysicsObject(-1,0,"../Models/bush1.obj");
    PhysicsObject * bush2 = new PhysicsObject(-1,0,"../Models/bush2.obj");
    std::pair<vector<glm::mat4>,PhysicsObject*> treeP1(tree1Model, tree1);
    worldDeco.push_back(treeP1);
    std::pair<vector<glm::mat4>,PhysicsObject*> treeP2(tree2Model, tree2);
    worldDeco.push_back(treeP2);
    std::pair<vector<glm::mat4>,PhysicsObject*> grassP1(grass1Model, grass1);
    worldDeco.push_back(grassP1);
    std::pair<vector<glm::mat4>,PhysicsObject*> grassP2(grass2Model, grass2);
    worldDeco.push_back(grassP2);
    std::pair<vector<glm::mat4>,PhysicsObject*> stoneP1(stone1Model, stone1);
    worldDeco.push_back(stoneP1);
    std::pair<vector<glm::mat4>,PhysicsObject*> stoneP2(stone2Model, stone2);
    worldDeco.push_back(stoneP2);
    std::pair<vector<glm::mat4>,PhysicsObject*> stoneP3(stone3Model, stone3);
    worldDeco.push_back(stoneP3);
    std::pair<vector<glm::mat4>,PhysicsObject*> stoneP4(stone4Model, stone4);
    worldDeco.push_back(stoneP4);
    std::pair<vector<glm::mat4>,PhysicsObject*> barkP1(bark1Model, bark1);
    worldDeco.push_back(barkP1);
    std::pair<vector<glm::mat4>,PhysicsObject*> barkP2(bark2Model, bark2);
    worldDeco.push_back(barkP2);
    std::pair<vector<glm::mat4>,PhysicsObject*> shroomP1(shroom1Model, shroom1);
    worldDeco.push_back(shroomP1);
    std::pair<vector<glm::mat4>,PhysicsObject*> shroomP2(shroom2Model, shroom2);
    worldDeco.push_back(shroomP2);
    std::pair<vector<glm::mat4>,PhysicsObject*> bushP1(bush1Model, bush1);
    worldDeco.push_back(bushP1);
    std::pair<vector<glm::mat4>,PhysicsObject*> bushP2(bush2Model, bush2);
    worldDeco.push_back(bushP2);
    for (int j = 0; j < worldDeco.size(); j++)
    {
        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, worldDeco[j].first.size() * sizeof(glm::mat4), &worldDeco[j].first[0], GL_STATIC_DRAW);
        for (unsigned int i = 0; i < worldDeco[j].second->getModel()->meshes.size(); i++)
        {
            unsigned int VAO =  worldDeco[j].second->getModel()->meshes[i]->VAO;
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
    }
}
