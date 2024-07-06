#pragma once
#include "../Gen/HeightMapGen.h"
#include "../Gen/FractualNoiseGen.h"
#include <vector>

class BaseTerrain {
protected:
	const float SIZE = 100.f;
	int terrainWidth;
	int terrainDepth;
	int vertexCount;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;  // VAO ID
	unsigned int VBO;  // VBO ID
	unsigned int EBO;  // EBO ID
public:
	//bool LoadHeightMap(const char* mapPath);
	BaseTerrain(int width, int depth);
	~BaseTerrain(void)
	{};
	void InitializeTerrain();
	bool GenerateVertices();
	void Bind();
	void Draw();

	std::vector<float>GetVertices() { return vertices; }
	std::vector<unsigned int>GetIndices() { return indices; }
	//unsigned int GetNumberOfVertices() const { return numOfVertices; }
	//unsigned int GetNumberOfIndices() const { return numOfIndices; }
	float getSize() { return SIZE; }
	unsigned int getVertexCount() { return vertexCount; }
};