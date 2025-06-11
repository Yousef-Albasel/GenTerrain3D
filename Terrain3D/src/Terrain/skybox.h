#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"

class Skybox {
private:
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTexture;
    Shader* skyboxShader;

    // Load a cubemap texture from 6 individual texture faces
    unsigned int loadCubemap(const std::vector<std::string>& faces);

public:
    Skybox(const std::vector<std::string>& faces, const std::string& shader_path);
    ~Skybox();

    void Draw(const glm::mat4& view,  glm::mat4& projection);
};