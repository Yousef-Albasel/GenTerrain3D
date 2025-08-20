#pragma once
#include <vector>
#include <VertexArray.h>
#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <ElementBuffer.h>
#include <iostream>

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
        // Safety checks
        if (vertices.empty() || texCoords.empty() || normals.empty() || indices.empty()) {
            std::cout << "ERROR: One or more input vectors are empty!" << std::endl;
            std::cout << "Vertices: " << vertices.size() << ", TexCoords: " << texCoords.size()
                << ", Normals: " << normals.size() << ", Indices: " << indices.size() << std::endl;
            return;
        }

        // Check if arrays have compatible sizes
        size_t numVertices = vertices.size() / 3;
        size_t numTexCoords = texCoords.size() / 2;
        size_t numNormals = normals.size() / 3;

        if (numVertices != numTexCoords || numVertices != numNormals) {
            std::cout << "ERROR: Array size mismatch!" << std::endl;
            std::cout << "Vertices: " << numVertices << ", TexCoords: " << numTexCoords
                << ", Normals: " << numNormals << std::endl;
            return;
        }

        std::cout << "Initializing buffers with " << numVertices << " vertices..." << std::endl;

        m_VAO.Bind();

        std::vector<float> combinedVertices;
        combinedVertices.reserve(numVertices * 8); // 3 pos + 2 tex + 3 normal = 8 floats per vertex

        for (size_t i = 0; i < numVertices; ++i) {
            // Check bounds before accessing
            size_t vertexIndex = i * 3;
            size_t texIndex = i * 2;
            size_t normalIndex = i * 3;

            if (vertexIndex + 2 >= vertices.size() ||
                texIndex + 1 >= texCoords.size() ||
                normalIndex + 2 >= normals.size()) {
                std::cout << "ERROR: Index out of bounds at vertex " << i << std::endl;
                return;
            }

            // Add vertex positions
            combinedVertices.push_back(vertices[vertexIndex + 0]);
            combinedVertices.push_back(vertices[vertexIndex + 1]);
            combinedVertices.push_back(vertices[vertexIndex + 2]);

            // Add texture coordinates
            combinedVertices.push_back(texCoords[texIndex + 0]);
            combinedVertices.push_back(texCoords[texIndex + 1]);

            // Add normals
            combinedVertices.push_back(normals[normalIndex + 0]);
            combinedVertices.push_back(normals[normalIndex + 1]);
            combinedVertices.push_back(normals[normalIndex + 2]);
        }

        std::cout << "Combined vertices size: " << combinedVertices.size() << std::endl;

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

        std::cout << "Buffers initialized successfully!" << std::endl;
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

    void Draw_Patches() const {
        m_VAO.Bind();
        glDrawElements(GL_PATCHES, m_EBO->GetCount(), GL_UNSIGNED_INT, nullptr);
        m_VAO.Unbind();
    };

private:
    VertexArray m_VAO;
    VertexBuffer* m_VBO;
    ElementBuffer* m_EBO;

    void Cleanup() {
        delete m_VBO;
        delete m_EBO;
        m_VBO = nullptr;
        m_EBO = nullptr;
    };
};