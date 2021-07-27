//
// Created by drive on 2/7/21.
//

#ifndef OPENGLTEST_MESH_H
#define OPENGLTEST_MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include "../../glm/glm.hpp"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../libs/stb_image.h"
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
    glm::vec3 ka,kd,ks;
};

unsigned int static TextureFromFile(const char *path, const std::string &directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}



class Mesh {
public:

    // mesh Data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    //Mesh transform
    glm::vec3 position;
    float angle;
    glm::vec3 axis;
    glm::vec3 scale;

    unsigned int VAO;

    ~Mesh();

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const char *path, std::string directory);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const char **path, uint nTextures);
    Mesh();
    void setModel(glm::mat4 model);
    void Draw(Shader &shader, bool outlined, unsigned int depthMap);

    void outlineMesh(Shader &outline, glm::vec3 scale);


private:
    glm::mat4 model;
    unsigned int VBO, EBO;
    void setupMesh();
};


#endif //OPENGLTEST_MESH_H
