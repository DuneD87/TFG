//
// Created by drive on 3/8/21.
//

#ifndef OPENGLTEST_PLANETCHUNK_H
#define OPENGLTEST_PLANETCHUNK_H

#include "../Entity.h"

class PlanetChunk {
public:
    PlanetChunk(int resolution, glm::vec3 localUp,glm::vec3 position ,std::vector<Texture> textures);
private:
    void setupMesh();
    std::vector<Texture> textures;
    std::vector<int> lod = {0,100,250,500,700,900,1100};
    bool isLeaf;
    std::vector<PlanetChunk*> children;
    int resolution;
    glm::vec3 position;
};


#endif //OPENGLTEST_PLANETCHUNK_H
