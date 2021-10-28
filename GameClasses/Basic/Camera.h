//
// Created by drive on 1/7/21.
//

#ifndef OPENGLTEST_CAMERA_H
#define OPENGLTEST_CAMERA_H
#include <glad/glad.h>
#include "../Util/glm/glm.hpp"
#include "../Util/glm/gtc/matrix_transform.hpp"
#include "../Util/glm/detail/type_quat.hpp"
#include "../Util/glm/ext/quaternion_common.hpp"

#include <vector>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    RLEFT,
    RRIGHT,
    UP,
    DOWN
};

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  20.5f;
const float SENSITIVITY =  0.001f;
const float ZOOM        =  45.0f;

class Camera {
public:
    // euler Angles
    float Yaw,oldYaw;
    float Pitch,oldPitch;
    float Roll,oldRoll;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::quat orientation;
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
private:

    void updateCameraVectors();
};


#endif //OPENGLTEST_CAMERA_H
