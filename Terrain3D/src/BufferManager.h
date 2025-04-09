#pragma once
#include <vector>
#include <VertexArray.h>
#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <ElementBuffer.h>

class BufferManager {
public:
    BufferManager() : m_VBO(nullptr), m_EBO(nullptr) {};
    ~BufferManager() { Cleanup(); };
    void InitializeBuffers(
        const std::vector<float>& vertices,
        const std::vector<float>& texCoords,
        const std::vector<float>& normals,
        const std::vector<unsigned int>& indices
    ) {
        m_VAO.Bind();

        std::vector<float> combinedVertices;
        combinedVertices.reserve(vertices.size() + texCoords.size() + normals.size());
        for (size_t i = 0; i < vertices.size() / 3; ++i) {
            // Add vertex positions
            combinedVertices.push_back(vertices[i * 3]);
            combinedVertices.push_back(vertices[i * 3 + 1]);
            combinedVertices.push_back(vertices[i * 3 + 2]);

            // Add texture coordinates
            combinedVertices.push_back(texCoords[i * 2]);
            combinedVertices.push_back(texCoords[i * 2 + 1]);

            // Add normals
            combinedVertices.push_back(normals[i * 3]);
            combinedVertices.push_back(normals[i * 3 + 1]);
            combinedVertices.push_back(normals[i * 3 + 2]);
        }

        m_VBO = new VertexBuffer(combinedVertices.data(), combinedVertices.size() * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3); // Vertex positions
        layout.Push<float>(2); // Texture coordinates
        layout.Push<float>(3); // Normals
        m_VAO.AddBuffer(*m_VBO, layout);

        // Create and bind the EBO
        m_EBO = new ElementBuffer(indices.data(), indices.size() * sizeof(unsigned int));

        m_VAO.Unbind();
        m_VBO->Unbind();
        m_EBO->Unbind();
    }

    void Bind() const {
        m_VAO.Bind();
    };

    void Unbind() const {
        m_VAO.Unbind();
    };

    void Draw() const {
        m_VAO.Bind();
        glDrawElements(GL_TRIANGLES, m_EBO->GetCount(), GL_UNSIGNED_INT, nullptr);
        m_VAO.Unbind();
    };

private:
    VertexArray m_VAO;
    VertexBuffer* m_VBO;
    ElementBuffer* m_EBO;

    void Cleanup() {
        delete m_VBO;
        delete m_EBO;
    };
};
