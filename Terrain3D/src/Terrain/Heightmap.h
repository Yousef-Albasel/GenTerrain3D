#pragma once
#include <vector>
class Heightmap {
protected:
	const float m_size;
	int m_width;
	int m_depth;

	int vertexCount;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

public:
	void InitializeHeightMap();
	bool GenerateVertices();
};