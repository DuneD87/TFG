//
// Created by drive on 2/7/21.
//

#ifndef OPENGLTEST_LIGHT_H
#define OPENGLTEST_LIGHT_H
#include <iostream>
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
enum Type {
    PointLight,
    SpotLight,
    dirLight
};
class Light {
public:
    Light(Type type,glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant,
          float linear, float quadratic);
    Type getType();
    void setPosition(glm::vec3 position);
    void setAmbient(glm::vec3 ambient);
    void setDiffuse(glm::vec3 diffuse);
    void setSpecular(glm::vec3 specular);
    void setDirection(glm::vec3 direction);
    void setCutOff(float cutOff);
    void setOuterCutOff(float outerCutOff);
private:
    glm::vec3 position,ambient,diffuse,specular,direction;
    Type type;
    float constant,linear,quadratic,cutOff,outerCutOff;
};


#endif //OPENGLTEST_LIGHT_H
