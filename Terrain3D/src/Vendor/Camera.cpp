#include "Camera.h"

// Camera member functions
void Camera::setCameraPos(glm::vec3 cp) {
    cameraPos = cp;
};

void Camera::setCameraFront(glm::vec3 cf) {
    cameraFront = cf;
};

void Camera::setCameraUp(glm::vec3 cu) {
    cameraUp = cu;
};

glm::mat4 Camera::setViewMat() {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
};

glm::mat4 Camera::setViewMat(glm::vec3 cp, glm::vec3 cf, glm::vec3 cu) {
    return glm::lookAt(cp, cp + cf, cu);
};

void Camera::ProcessInputs(GLFWwindow* window, float cameraSpeed, float deltaTime) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    mouse_callback(window, mouseX, mouseY);
    cameraSpeed *= deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) // UP
        cameraPos += cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // DOWN
        cameraPos -= cameraUp * cameraSpeed;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void Camera::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    ProcessMouseMovement(xoffset, yoffset,true);
}
