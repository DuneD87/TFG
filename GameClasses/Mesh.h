//
// Created by drive on 2/7/21.
//

#ifndef OPENGLTEST_MESH_H
#define OPENGLTEST_MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#include "Shader.h"

#include <string>
#include <vector>


struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};


class Mesh {
public:
    // mesh Data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    unsigned int VAO;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader &shader);
private:
    unsigned int VBO, EBO;
    void setupMesh();
};


#endif //OPENGLTEST_MESH_H
