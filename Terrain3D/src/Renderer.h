#pragma once
#include "GL/glew.h"
#include "Terrain/CTerrain.h"
#include <vector>
#include "Vendor/Shader.h"
#include "Vendor/Camera.h"
#include "Vendor/Window.h"
class Renderer
{
    CTerrain& m_terrain;
    Shader *shader;
    Camera m_camera;
public:
    Renderer(CTerrain& terrain);
    ~Renderer();
    
    void processInput(Window &window,float speed, float deltaTime);
    void Init();
    void Render();
    void Clean();
private:
    unsigned int VAO, VBO, EBO;
};

