#pragma once
#include "GL/glew.h"
#include "Terrain/BaseTerrain.h"
#include <glm/glm.hpp>
#include <vector>
#include "Vendor/Shader.h"
#include "Vendor/Camera.h"
#include "Vendor/Window.h"
#include "../Models/Cube.h"
#include "../Terrain/BaseTerrain.h"
class Renderer
{
    Cube* cube1;
    float lastX = 800 / 2.0f;
    float lastY = 600/ 2.0f;
    double mouseX=0.0, mouseY=0.0;
    bool firstMouse = true;
    Camera *camera = new Camera(glm::vec3(0.f, 5.f, 5.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f));
    Shader *shader = new Shader("Shaders/basic.shader");
    BaseTerrain terrain{10, 10};
public:

    Renderer() {

    };
    ~Renderer() {
        Clean();
    };
 
    void Init();
    void Render();
    void Clean();
    void processKeyboardInput(GLFWwindow* window, float deltaTime);
    void ProcessMouseInput(GLFWwindow* window,const float &dt);
private:
    unsigned int VAO, VBO, EBO;
};

