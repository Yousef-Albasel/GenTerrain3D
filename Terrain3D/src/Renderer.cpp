#include "Renderer.h"
#include "../Models/Cube.h"
void Renderer::Init() {
    terrain.InitializeTerrain();
    terrain.CreateMidPointDisplacement(1.4f, 0.f, 300.f);
    int textureUnits = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
    printf("%d", textureUnits);
    glEnable(GL_TEXTURE_2D);
    std::vector<std::string> faces{
    "Skymap/right.png",
    "Skymap/left.png",
    "Skymap/top.png",
    "Skymap/bottom.png",
    "Skymap/front.png",
    "Skymap/back.png"
    };

    skybox = new Skybox(faces, "Shaders/skybox.shader");
}


void Renderer::Render() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    rotationAngle += 0.01f;
    if (rotationAngle >= 360.0f) rotationAngle = 0.0f;

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
    // Set fog parameters
    shader.SetUniform3f("fogColor", 0.75f, 0.85f, 0.9f);
    shader.SetUniform1f("fogStart", 300.0f);
    shader.SetUniform1f("fogEnd", 1500.0f);
    shader.SetUniform1i("fogEquation", 2); 

    glm::vec3 lightPos = glm::vec3(50.0f, 500.0f, 50.0f);                // Light's position in world space
    glm::vec3 baseDir = glm::vec3(-1.0f, -1.0f, -1.0f);
    glm::vec3 gReversedLightDir = -baseDir;

    shader.SetUniform3f("gReversedLightDir", gReversedLightDir.x, gReversedLightDir.y, gReversedLightDir.z);
    terrain.Bind();
    terrain.Draw();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    skybox->Draw(view, projection);

}

void Renderer::Clean() {
    delete skybox;
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