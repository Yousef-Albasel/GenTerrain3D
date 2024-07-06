#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "BaseTerrain.h"
#include <iostream>
//BaseTerrain::BaseTerrain(int width, int depth) : terrainWidth(width), terrainDepth(depth){
//};

BaseTerrain::BaseTerrain(int width, int depth) : terrainWidth(width), terrainDepth(depth)
{
	vertexCount = terrainWidth * terrainDepth;
	vertices.resize(vertexCount * 3);
	indices.resize((terrainWidth - 1) * (terrainDepth - 1) * 6);
    InitializeTerrain();
}

void BaseTerrain::InitializeTerrain()
{
    GenerateVertices();
    int indexCount = indices.size();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create and bind VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Create and bind EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    // Assuming position attribute is at index 0 with 3 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO, VBO, and EBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}



bool BaseTerrain::GenerateVertices()
{
	int idx = 0;
    for (int i = 0; i < terrainDepth; ++i) {
        for (int j = 0; j < terrainWidth; ++j) {
			vertices[idx * 3] = static_cast<float>(j) / (terrainWidth - 1) * SIZE;
			vertices[idx * 3 + 1];
			vertices[idx* 3 + 2] = static_cast<float>(i) / (terrainDepth - 1) * SIZE;
			idx++;
        }
    }

    int pointer = 0;
    for (int gz = 0; gz < terrainDepth - 1; ++gz) {
        for (int gx = 0; gx < terrainWidth - 1; ++gx) {

            int topLeft = (gz * terrainWidth) + gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz + 1) * terrainWidth) + gx;
            int bottomRight = bottomLeft + 1;

            indices[pointer++] = topLeft;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = topRight;
            indices[pointer++] = topRight;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = bottomRight;
        }
    }

	return true;
}

void BaseTerrain::Bind() {
    glBindVertexArray(VAO);
}

void BaseTerrain::Draw() {

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

}