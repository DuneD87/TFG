//
// Created by drive on 2/7/21.
//

#ifndef OPENGLTEST_MODEL_H
#define OPENGLTEST_MODEL_H
#include <glad/glad.h>

#include "../../glm/glm.hpp"
#include "../../glm/gtc/matrix_transform.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model {
public:
    // model data
    std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
std::vector<Mesh*>    meshes;
    std::string directory;
    bool gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    Model(std::string const &path, bool gamma = false);
    Model();
    ~Model();
    void Draw(Shader &shader, bool outlined = false, unsigned int depthMap = -1);
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    void setPosition(glm::vec3 position);
    void setRotation(float angle, glm::vec3 axis);
    void setScale(glm::vec3 scale);
    void setModel(glm::mat4 model);
    void outlineObject(Shader &outline, glm::vec3 scale);
private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const &path);
    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.


};


#endif //OPENGLTEST_MODEL_H
