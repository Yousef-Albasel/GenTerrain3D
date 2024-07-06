#pragma once
#pragma once
#include <GLFW/glfw3.h>
#include "Camera.h"

class InputManager {
    Camera& camera;
    float lastX, lastY;
    bool firstMouse;

public:
    InputManager(Camera& cam);

    void processInput(GLFWwindow* window, float speed, float deltaTime);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
};


InputManager::InputManager(Camera& cam) : camera(cam), lastX(400), lastY(300), firstMouse(true) {}

void InputManager::processInput(GLFWwindow* window,float speed, float deltaTime) {
    // Keyboard input
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboardInputs(GLFW_KEY_W, speed ,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboardInputs(GLFW_KEY_S, speed, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboardInputs(GLFW_KEY_A, speed, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboardInputs(GLFW_KEY_D, speed, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKeyboardInputs(GLFW_KEY_SPACE, speed, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.processKeyboardInputs(GLFW_KEY_LEFT_SHIFT, speed, deltaTime);

    // Mouse input
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void InputManager::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
}
