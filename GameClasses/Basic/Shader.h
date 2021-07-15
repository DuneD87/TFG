//Basic
// Created by drive on 24/6/21.
//

#ifndef OPENGLTEST_SHADER_H
#define OPENGLTEST_SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include "../../glm/glm.hpp"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/type_ptr.hpp"
#include "../Entities/Light.h"
#include <string>
#include <vector>
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
    void addLights(std::vector<Light> lights);
    void addSpotLight(Light light, glm::vec3 direction, glm::vec3 position,float cutOff, float outerCutOff);
    void disableSpotLight();
private:
    int nPointLights;
    void checkCompileErrors(unsigned int shader, std::string type);
};


#endif //OPENGLTEST_SHADER_H
