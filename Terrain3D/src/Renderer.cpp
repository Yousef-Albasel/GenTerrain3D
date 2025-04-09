#include "Renderer.h"
#include "../Models/Cube.h"
void Renderer::Init() {
    terrain.InitializeTerrain();
    terrain.CreateMidPointDisplacement(1.5f, 0.f, 50.f);
    int textureUnits = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
    printf("%d", textureUnits);
    glEnable(GL_TEXTURE_2D);

}


void Renderer::Render() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.01f, 10000.0f);

    float minHeightLoc = terrain.getMinHeightLoc();
    float maxHeightLoc = terrain.getMaxHeightLoc();

    shader.Bind();
    shader.setMVP(model, view, projection);
    shader.SetUniform1f("minHeight", minHeightLoc);
    shader.SetUniform1f("maxHeight", maxHeightLoc);
    shader.SetUniform1i("texture1", 0); // Bind texture1 to texture unit 0
    shader.SetUniform1i("texture2", 1); // Bind texture2 to texture unit 1
    shader.SetUniform1i("texture3", 2); // Bind texture3 to texture unit 2
    shader.SetUniform1i("texture4", 3); // Bind texture4 to texture unit 3

    glm::vec3 lightPos = glm::vec3(10.0f, 100.0f, 10.0f); // Light's position in world space
    glm::vec3 lightDir = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)); // Light direction (fixed or dynamic)
    glm::vec3 gReversedLightDir = -lightDir; // Reverse the direction of the light


    shader.SetUniform3f("gReversedLightDir", gReversedLightDir.x, gReversedLightDir.y, gReversedLightDir.z);
    terrain.Bind();
    terrain.Draw();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void Renderer::Clean() {
    // Clean up resources
}


void Renderer::processKeyboardInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //std::cout << "Moving Forward\n";
        camera.updateKeyboardInput(deltaTime, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //std::cout << "Moving Backward\n";
        camera.updateKeyboardInput(deltaTime, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        //std::cout << "Moving Left\n";
        camera.updateKeyboardInput(deltaTime, 3);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        //std::cout << "Moving Right\n";
        camera.updateKeyboardInput(deltaTime, 2);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // DOWN
        camera.updateKeyboardInput(deltaTime, 4);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) // UP
        camera.updateKeyboardInput(deltaTime, 5);

    //std::cout << "Camera Position: "
    //    << camera.getCameraPos().x << ", "
    //    << camera.getCameraPos().y << ", "
    //    << camera.getCameraPos().z << std::endl;
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

    camera.updateMouseInput(dt,xoffset, yoffset);
}