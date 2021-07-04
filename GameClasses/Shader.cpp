//
// Created by drive on 24/6/21.
//

#include "Shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    nPointLights = 0;
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMat4(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setVec3(const std::string &name, glm::vec3 value) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()),value.x,value.y,value.z);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Shader::addLights(std::vector<Light> lights) {
    for (int i = 0; i < lights.size();i++) {
        if (lights[i].getType() == PointLight) {
            this->setVec3("pointLights["+std::to_string(i)+"].position", lights[i].getPosition());
            this->setVec3("pointLights["+std::to_string(i)+"].ambient",lights[i].getAmbient());
            this->setVec3("pointLights["+std::to_string(i)+"].diffuse", lights[i].getDiffuse());
            this->setVec3("pointLights["+std::to_string(i)+"].specular",lights[i].getSpecular());
            this->setFloat("pointLights["+std::to_string(i)+"].constant", lights[i].getConstant());
            this->setFloat("pointLights["+std::to_string(i)+"].linear", lights[i].getLinear());
            this->setFloat("pointLights["+std::to_string(i)+"].quadratic", lights[i].getQuadratic());
            //lights[i].toString();
        }
    }
}

void Shader::addSpotLight(Light light, glm::vec3 direction, glm::vec3 position,float cutOff, float outerCutOff) {
    this->setVec3("spotLight.position", position);
    this->setVec3("spotLight.direction", direction);
    this->setVec3("spotLight.ambient", light.getAmbient());
    this->setVec3("spotLight.diffuse", light.getDiffuse());
    this->setVec3("spotLight.specular", light.getSpecular());
    this->setFloat("spotLight.constant", light.getConstant());
    this->setFloat("spotLight.linear", light.getLinear());
    this->setFloat("spotLight.quadratic", light.getQuadratic());
    this->setFloat("spotLight.cutOff", cutOff);
    this->setFloat("spotLight.outerCutOff", outerCutOff);
}

void Shader::disableSpotLight() {
    this->setVec3("spotLight.ambient", glm::vec3(0));
    this->setVec3("spotLight.diffuse", glm::vec3(0));
    this->setVec3("spotLight.specular", glm::vec3(0));
    this->setFloat("spotLight.constant", 0);
    this->setFloat("spotLight.linear", 0);
    this->setFloat("spotLight.quadratic", 0);
    this->setFloat("spotLight.cutOff", 0);
    this->setFloat("spotLight.outerCutOff", 0);
}
