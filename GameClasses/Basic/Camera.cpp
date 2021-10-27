//
// Created by drive on 1/7/21.
//

#include <iostream>
#include "Camera.h"
#include "../Util/glm/detail/type_quat.hpp"
#include "../Util/glm/gtc/quaternion.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(1.0f, 0.0f, 1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    Roll = 90;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{

    glm::mat4 rotate = glm::mat4_cast(orientation);

    glm::mat4 translate = glm::mat4(1.0f);
    translate = glm::translate(translate, -Position);

    return rotate * translate;
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == UP)
        Position += Up * velocity;
    if (direction == DOWN)
        Position -= Up * velocity;
    if (direction == RLEFT)
        Roll += (SENSITIVITY)*1000*deltaTime;
    if (direction == RRIGHT)
        Roll -= (SENSITIVITY)*1000*deltaTime;
    updateCameraVectors();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 180.0f)
            Pitch = -180.0f;
        if (Pitch < -180.0f)
            Pitch = 180.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}


void Camera::updateCameraVectors() {

    glm::quat qPYR = glm::quat(glm::vec3(-Pitch, Yaw, -Roll));
    // reset values
    Pitch = Yaw = Roll = 0;

    // update qCamera
    orientation = qPYR * orientation;
    orientation = glm::normalize(orientation);

    Front = glm::conjugate(orientation) * glm::vec3(0.0f, 0.0f, -1.0f);
    // also re-calculate the Right and Up vector
    Right = glm::conjugate(orientation) * glm::vec3(1.0f, 0.0f, 0.0f);
    Up = glm::conjugate(orientation) * glm::vec3(0.0f, 1.0f, 0.0f);
}
