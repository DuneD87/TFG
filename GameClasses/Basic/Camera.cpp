//
// Created by drive on 1/7/21.
//

#include "Camera.h"
#include "../../glm/detail/type_quat.hpp"
#include "../../glm/gtc/quaternion.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(1.0f, 0.0f, 1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    Roll = 0;
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
    //FPS camera:  RotationX(pitch) * RotationY(yaw)
    glm::quat qPitch = glm::angleAxis(-Pitch, glm::vec3(1, 0, 0));
    glm::quat qYaw = glm::angleAxis(Yaw, glm::vec3(0, 1, 0));
    glm::quat qRoll = glm::angleAxis(Roll,glm::vec3(0,0,1));

    //For a FPS camera we can omit roll
    orientation =  qPitch * qYaw;
    orientation = orientation * qRoll;
    orientation = glm::normalize(orientation);
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
        Roll += (SENSITIVITY*100)*deltaTime;
    if (direction == RRIGHT)
        Roll -= (SENSITIVITY*100)*deltaTime;
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
    glm::vec3 front;
    front.x = orientation.x;
    front.y = orientation.y;
    front.z = orientation.z;

    Front = glm::conjugate(orientation) * glm::vec3(0.0f, 0.0f, -1.0f);
    // also re-calculate the Right and Up vector
    Right = glm::conjugate(orientation) * glm::vec3(1.0, 0.0f, 0.0f);
    Up = glm::conjugate(orientation) * glm::vec3(0.0f, 1.0f, 0.0f);
}
