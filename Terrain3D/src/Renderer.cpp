#include "Renderer.h"
#include "Vendor/Window.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
Renderer::Renderer(CTerrain& terrain) : m_terrain(terrain), VAO(0), VBO(0), EBO(0) {};

void Renderer::Init(){
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glBindVertexArray(VAO));
    std::vector<float>vertices = m_terrain.getVBO();
    std::vector<unsigned int>indices= m_terrain.getEBO();
    GLCall(glGenBuffers(1, &VBO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW));

    // position attribute
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glGenBuffers(1, &EBO));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),&indices[0],GL_STATIC_DRAW));
    shader = new Shader("Shaders/basic.shader");
    
}
void Renderer::processInput(Window &window,float speed,float deltaTime) {
    m_camera.ProcessInputs(window.GetWindow(), speed,deltaTime);
}
void Renderer::Render() {
    // Create Shader
    shader->Bind();

    glm::mat4 model = glm::mat4(1.0f);
    shader->setUniformMatrix4fv("model", 1, GL_FALSE, model);

    // View Matrix Setup
    glm::mat4 view = glm::mat4(1.0f);
    view = m_camera.setViewMat();
    shader->setUniformMatrix4fv("view", 1, GL_FALSE, view);
    //glm::vec3 CameraPos = m_camera.getCameraPos();
    // Projection Matrix setup
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(80.0f), 640.0f / 480.0f, 0.1f, 1000.0f);
    shader->setUniformMatrix4fv("projection", 1, GL_FALSE, projection);


    GLCall(glBindVertexArray(VAO));
    const unsigned int NUM_STRIPS = m_terrain.getNStrips();
    const unsigned int NUM_VERTS_PER_STRIP = m_terrain.getNVertices();
    for (unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
    {
        GLCall(glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
            NUM_VERTS_PER_STRIP, // number of indices to render
            GL_UNSIGNED_INT,     // index data type
            (void*)(sizeof(unsigned int)
                * NUM_VERTS_PER_STRIP
                * strip))); // offset to starting index
    }
}


void Renderer::Clean() {
    // Delete buffers and VAO
    GLCall(glDeleteVertexArrays(1, &VAO));
    GLCall(glDeleteBuffers(1, &VBO));
    GLCall(glDeleteBuffers(1, &EBO));
}

Renderer::~Renderer() {
    Clean();
}
