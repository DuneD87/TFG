//
// Created by drive on 2/7/21.
//

#ifndef OPENGLTEST_LIGHT_H
#define OPENGLTEST_LIGHT_H
#include <iostream>
#include "../../glm/glm.hpp"
#include "../../glm/gtc/matrix_transform.hpp"
#include "Entity.h"

class Light : public Entity {
public:
    Light(std::string lightType, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant,
          float linear, float quadratic, int entId);
    Light(std::string lightType, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,glm::vec3 direction, int entId);
    std::string getSubType();
    glm::vec3 getPosition() override;
    glm::vec3 getDirection();
    glm::vec3 getAmbient();
    glm::vec3 getDiffuse();
    glm::vec3 getSpecular();
    float getConstant();
    float getLinear();
    float getQuadratic();
    void setPosition(glm::vec3 position);
    void setAmbient(glm::vec3 ambient);
    void setDiffuse(glm::vec3 diffuse);
    void setSpecular(glm::vec3 specular);
    void setDirection(glm::vec3 direction);
    void setCutOff(float cutOff);
    void setOuterCutOff(float outerCutOff);
    void draw(Shader &shader,bool outlined, int depthMap);
    std::string toString();
private:
    glm::vec3 position,ambient,diffuse,specular,direction;
    std::string subType;
    int lightIndex;
    float constant,linear,quadratic,cutOff,outerCutOff;
};


#endif //OPENGLTEST_LIGHT_H
