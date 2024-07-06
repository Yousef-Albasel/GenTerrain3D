#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Vendor/window.h"
#include "Renderer.h"
#include <vector>


int main(void) {
    Window window(800, 600, "Terrain Generator");

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    Renderer renderer;
    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);
        window.calculateDeltaTime(deltaTime, lastFrame);

        renderer.Init();
        renderer.Render();
        renderer.processKeyboardInput(window.GetWindow(),deltaTime);
        renderer.ProcessMouseInput(window.GetWindow(),deltaTime);

        glfwSwapBuffers(window.GetWindow());
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
