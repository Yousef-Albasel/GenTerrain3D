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
#include "../Terrain/FaultFormationTerrain.h"
#include "../Terrain/MPDTerrain.h"
class Renderer
{
    MPDTerrain terrain{ 128, 128, 1024};
    float lastX = 800 / 2.0f;
    float lastY = 600/ 2.0f;
    double mouseX=0.0, mouseY=0.0;
    float rotationAngle = 0.0f;
    bool firstMouse = true;
    Camera camera{ glm::vec3(150.f, 300.f, 50.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f) };
    Shader shader{ "Shaders/basic.shader" };

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
    //unsigned int VAO, VBO, EBO;
};

