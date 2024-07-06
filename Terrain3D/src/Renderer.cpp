#include "Renderer.h"
#include "../Models/Cube.h"
void Renderer::Init() {
}


void Renderer::Render() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    terrain.Bind();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

    shader->Bind();
    shader->setMVP(model, view, projection);
    terrain.Draw();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void Renderer::Clean() {
    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete camera;
    delete shader;
}


void Renderer::processKeyboardInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //std::cout << "Moving Forward\n";
        camera->updateKeyboardInput(deltaTime, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //std::cout << "Moving Backward\n";
        camera->updateKeyboardInput(deltaTime, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        //std::cout << "Moving Left\n";
        camera->updateKeyboardInput(deltaTime, 3);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        //std::cout << "Moving Right\n";
        camera->updateKeyboardInput(deltaTime, 2);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // DOWN
        camera->updateKeyboardInput(deltaTime, 4);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) // UP
        camera->updateKeyboardInput(deltaTime, 5);

    //std::cout << "Camera Position: "
    //    << camera->getCameraPos().x << ", "
    //    << camera->getCameraPos().y << ", "
    //    << camera->getCameraPos().z << std::endl;
}

void Renderer::ProcessMouseInput(GLFWwindow* window,const float &dt)
{
    glfwGetCursorPos(window, &mouseX, &mouseY);
    //std::cout << mouseX << " : " << mouseY << std::endl;
    float xpos = static_cast<float>(mouseX);
    float ypos = static_cast<float>(mouseY);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;

    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera->updateMouseInput(dt,xoffset, yoffset);
}