//
// Created by drive on 2/7/21.
//

#ifndef OPENGLTEST_LIGHT_H
#define OPENGLTEST_LIGHT_H
#include <iostream>
#include "../Util/glm/glm.hpp"
#include "../Util/glm/gtc/matrix_transform.hpp"
#include "Entity.h"

class Light : public Entity {
public:
    virtual ~Light();

public:
    Light(std::string lightType, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant,
          float linear, float quadratic, int entId, int lightIndex);
    Light(std::string lightType, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,glm::vec2 orientation, int entId);
    std::string getSubType();
    glm::vec2 getDirection();
    glm::vec3 getAmbient();
    glm::vec3 getDiffuse();
    glm::vec3 getSpecular();
    glm::vec3 getDirVector();
    float getConstant();
    float getLinear();
    float getQuadratic();
    void setAmbient(glm::vec3 ambient);
    void setDiffuse(glm::vec3 diffuse);
    void setSpecular(glm::vec3 specular);
    void setDirection(glm::vec2 orientation);
    void setCutOff(float cutOff);
    void setOuterCutOff(float outerCutOff);
    void draw(Shader &shader);
    std::string toString();
private:
    glm::vec3 position,ambient,diffuse,specular;
    glm::vec2 orientation;
    std::string subType;
    int lightIndex;
    float constant,linear,quadratic,cutOff,outerCutOff;
};


#endif //OPENGLTEST_LIGHT_H
