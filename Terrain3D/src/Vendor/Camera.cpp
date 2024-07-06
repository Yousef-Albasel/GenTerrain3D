#include "Camera.h"

// Camera member functions

void Camera::updateKeyboardInput(const float& dt, const int direction)
{
    switch (direction) {
    case 0:
        cameraPos += cameraFront * movementSpeed * dt;
        break;
    case 1:
        cameraPos -= cameraFront * movementSpeed * dt;
        break;
    case 2:
        cameraPos += cameraRight * movementSpeed * dt;
        break;
    case 3:
        cameraPos -= cameraRight * movementSpeed * dt;
        break;
    case 4:
        cameraPos -= cameraUp * movementSpeed * dt;
        break;
    case 5:
        cameraPos += cameraUp * movementSpeed * dt;
        break;
    default:
        break;
    }
}

void Camera::updateMouseInput(const float dt, const double& offsetX, const double& offsetY) {
    // update pitch and yaw

    this->pitch += static_cast<float>(offsetY) * this->sensitivity * dt;
    this->yaw += static_cast<float>(offsetX) * this->sensitivity * dt;

    // Constrains on pitch and yaw
    if (pitch > 80.f)
        pitch = 80.f;
    else if (pitch < -80.f)
        pitch = -80.f;
    if (yaw > 360.f || yaw < -360.f)
        yaw = 0.f;
    updateCameraVectors();

};

void Camera::updateCameraVectors() {
    cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront.y = sin(glm::radians(pitch));
    cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(cameraFront);
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
};
