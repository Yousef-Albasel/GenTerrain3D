#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Vendor/window.h"
#include "Renderer.h"
#include "Terrain/CTerrain.h"
#include "Vendor/Camera.h"

// Global camera instance
Camera camera;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.mouse_callback(window, xpos, ypos);
}

int main(void) {
    Window window(640, 480, "Hello World");
    CTerrain mterrain;
    if (!mterrain.LoadHeightMap("Heightmaps/iceland_heightmap.png"))
        return -1;

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.GetWindow(), mouse_callback);

    Renderer renderer(mterrain);
    renderer.Init();

    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float cameraSpeed = 2.5f;

        renderer.Render();
        renderer.processInput(window,cameraSpeed,deltaTime);

        glfwSwapBuffers(window.GetWindow());
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
