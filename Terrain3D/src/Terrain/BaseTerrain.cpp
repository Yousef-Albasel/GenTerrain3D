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
}

void BaseTerrain::InitializeTerrain()
{
    GenerateVertices();
    int indexCount = indices.size();
    bm.InitializeBuffers();
}



bool BaseTerrain::GenerateVertices()
{
	int idx = 0;
    for (int i = 0; i < terrainDepth; ++i) {
        for (int j = 0; j < terrainWidth; ++j) {
			vertices[idx * 3] = static_cast<float>(j) / (terrainWidth - 1) * SIZE;
			vertices[idx * 3 + 1] = 0.f;
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
    bm.Bind();
}

void BaseTerrain::Draw() {
    bm.Draw();
}
float BaseTerrain::GetHeightAt(int x, int z) const {
    if (x < 0 || x >= terrainWidth || z < 0 || z >= terrainDepth) {
        std::cerr << "Error: Coordinates out of bounds." << std::endl;
        return 0.0f;
    }
    int index = (z * terrainWidth + x) * 3 + 1; // y-coordinate is at offset 1
    return vertices[index];
}

void BaseTerrain::SetHeightAt(int x, int z, float height) {
    if (x < 0 || x >= terrainWidth || z < 0 || z >= terrainDepth) {
        std::cerr << "Error: Coordinates out of bounds." << std::endl;
        return;
    }
    int index = (z * terrainWidth + x) * 3 + 1; // y-coordinate is at offset 1
    vertices[index] = height;


}