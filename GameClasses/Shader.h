//
// Created by drive on 24/6/21.
//

#ifndef OPENGLTEST_SHADER_H
#define OPENGLTEST_SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, glm::vec3 value) const;
    void setMat4(const std::string &name,glm::mat4 value) const;
    //HERENCIAAAA classe Light implementa dirLight,pointLight i spotLight
    void addDirLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
    void addPointLight(unsigned int index, glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec,
                       float constant, float linear, float quadratic);
    void addSpotLight(unsigned int index, glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec,
                         float constant, float linear, float quadratic, float cutOff, float outerCutOff);
private:
    void checkCompileErrors(unsigned int shader, std::string type);
};


#endif //OPENGLTEST_SHADER_H
