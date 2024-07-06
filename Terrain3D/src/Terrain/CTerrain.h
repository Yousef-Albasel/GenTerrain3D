#pragma once
#include "../Gen/HeightMapGen.h"
#include <vector>

class CTerrain {
protected:
	std::vector<float>vertices;
	std::vector<unsigned int> indicies;
	unsigned int numOfStrips;
	unsigned int verticesPerStrip;
public:
	//virtual void Render(void) = 0;
	bool LoadHeightMap(const char* mapPath);
	//bool UnloadHeightMap();
	bool GenerateVertices(HeightMapGen& h_generator, int width, int height, int nChannels);
	CTerrain(void)
	{ };
	~CTerrain(void)
	{ };

	std::vector<float>getVBO() { return vertices; }
	std::vector<unsigned int>getEBO() { return indicies; }
	unsigned int getNStrips() { return numOfStrips; }
	unsigned int getNVertices() { return verticesPerStrip; }
};