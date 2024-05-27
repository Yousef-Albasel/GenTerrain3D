#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Window.h"
#include "stb_image.h"
#include "shader.h"
#include "VertexArray.h"
#include "ElementBuffer.h"
#include "Camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Tests/TestClearColor.h"
#include "Tests/SimpleCube.h"
#include "Tests/SimpleLighting.h"
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static Camera* g_Camera;
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "AlbaselStudios", NULL, NULL);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }


    //float deltaTime = 0.0f;	// Time between current frame and last frame
    //float lastFrame = 0.0f; // Time of last frame


    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 330");

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    test::Test* currentTest = NULL;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    /*testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::SimpleCube>("Simple Cube");
    */testMenu->RegisterTest<test::SimpleLighting>("Simple Lighting Cube");

    /* =========== Main Loop ============= */
   glEnable(GL_DEPTH_TEST);
   while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        currentTest->getWindowReference(window);
        // Delta Time setup
        //float currentFrame = glfwGetTime();
        //deltaTime = currentFrame - lastFrame;
        //lastFrame = currentFrame;

        if (currentTest) {
            currentTest->onUpdate(0.0f);
            currentTest->onRender();
            ImGui::Begin("Test Menu");
            if (currentTest != testMenu && ImGui::Button("<-")) {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->onImGuiRender();
            ImGui::End();
        }
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
   ImGui_ImplGlfw_Shutdown();
   ImGui_ImplOpenGL3_Shutdown();
   ImGui::DestroyContext();
   glfwTerminate();

    return 0;
}

