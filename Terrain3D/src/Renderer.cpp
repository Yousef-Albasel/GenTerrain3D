#include "Renderer.h"
#include "../Models/Cube.h"
void Renderer::Init() {
    terrain.InitializeTerrain();
    // Use tessellation version
    terrain.CreateMidPointDisplacementTes(2.0f, 0.0f, 300.0f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    int textureUnits = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
    printf("Available texture units: %d\n", textureUnits);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    // Set patch vertices to 4 for quad patches
    glPatchParameteri(GL_PATCH_VERTICES, 4);

    // Initialize tessellation shader - fix the file path
    tessellationShader = new Shader(
        "Shaders/tessellation_vertex.shader",   // Note: using the actual filename (missing 's')
        "Shaders/tessellation_vertex.shader",   // Same file (dummy parameter)
        "Shaders/TCS.shader",                  // Tessellation Control Shader
        "Shaders/TES.shader"                   // Tessellation Evaluation Shader
    );

    // Check if shader creation was successful
    if (tessellationShader == nullptr) {
        std::cerr << "Failed to create tessellation shader!" << std::endl;
        return;
    }

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

    rotationAngle += 0.01f;
    if (rotationAngle >= 360.0f) rotationAngle = 0.0f;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.01f, 10000.0f);

    float minHeightLoc = terrain.getMinHeightLoc();
    float maxHeightLoc = terrain.getMaxHeightLoc();

    // DEBUG: Print height values every few frames
    static int frameCount = 0;
    if (frameCount % 120 == 0) { // Every 2 seconds at 60fps
        std::cout << "=== RENDER DEBUG INFO ===" << std::endl;
        std::cout << "Frame: " << frameCount << std::endl;
        std::cout << "minHeightLoc: " << minHeightLoc << std::endl;
        std::cout << "maxHeightLoc: " << maxHeightLoc << std::endl;
        std::cout << "Height difference: " << (maxHeightLoc - minHeightLoc) << std::endl;
        std::cout << "Camera pos: " << camera.getCameraPos().x << ", "
            << camera.getCameraPos().y << ", " << camera.getCameraPos().z << std::endl;
    }
    frameCount++;

    // Use tessellation shader
    tessellationShader->Bind();

    // Set individual matrices for tessellation shaders
    tessellationShader->SetUniformMat4f("model", model);
    tessellationShader->SetUniformMat4f("view", view);
    tessellationShader->SetUniformMat4f("projection", projection);

    tessellationShader->SetUniform1f("minHeight", minHeightLoc);
    tessellationShader->SetUniform1f("maxHeight", maxHeightLoc);

    // Set textures
    tessellationShader->SetUniform1i("texture1", 0);
    tessellationShader->SetUniform1i("texture2", 1);
    tessellationShader->SetUniform1i("texture3", 2);
    tessellationShader->SetUniform1i("texture4", 3);

    // Set fog parameters
    tessellationShader->SetUniform3f("fogColor", 0.75f, 0.85f, 0.9f);
    tessellationShader->SetUniform1f("fogStart", 300.0f);
    tessellationShader->SetUniform1f("fogEnd", 1500.0f);
    tessellationShader->SetUniform1i("fogEquation", 2);

    // Tessellation uniforms
    tessellationShader->SetUniform1f("tessellationLevel", 8.0f);
    tessellationShader->SetUniform3f("cameraPos", camera.getCameraPos().x,
        camera.getCameraPos().y, camera.getCameraPos().z);
    tessellationShader->SetUniform1f("maxDistance", 1000.0f);

    // IMPORTANT: Set heightScale properly
    float heightScale = maxHeightLoc - minHeightLoc;
    tessellationShader->SetUniform1f("heightScale", heightScale);

    // DEBUG: Print heightScale
    if (frameCount % 120 == 0) {
        std::cout << "heightScale uniform: " << heightScale << std::endl;
    }

    // Bind heightmap texture
    tessellationShader->SetUniform1i("heightmapTexture", 4);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, terrain.getHeightmapTextureID());

    // Bind terrain and textures
    terrain.Bind();

    // Lighting
    glm::vec3 baseDir = glm::vec3(-1.0f, -1.0f, -1.0f);
    glm::vec3 gReversedLightDir = -baseDir;
    tessellationShader->SetUniform3f("gReversedLightDir", gReversedLightDir.x, gReversedLightDir.y, gReversedLightDir.z);

    // Draw using patches
    terrain.Draw_Patches();

    tessellationShader->Unbind();

    // Draw skybox
    skybox->Draw(view, projection);
}
void Renderer::Clean() {
    delete skybox;
    delete tessellationShader;
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