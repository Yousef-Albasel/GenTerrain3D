#pragma once
#include <vector>
struct HEIGHT_Map
{
	std::vector<std::vector<float>> HeightMapData;
	int height;
	int width;
	int m_iSize;
};

class CTerrain {
protected:
	std::vector<float>vertices;
	std::vector<unsigned int> indicies;
	HEIGHT_Map heightmap;
	unsigned int numOfStrips;
	unsigned int verticesPerStrip;
public:
	//virtual void Render(void) = 0;
	bool LoadHeightMap(const char* mapPath);
	//bool UnloadHeightMap();
	bool GenerateVertices();
;	CTerrain(void)
	{ };
	~CTerrain(void)
	{ };

	std::vector<float>getVBO() { return vertices; }
	std::vector<unsigned int>getEBO() { return indicies; }
	unsigned int getNStrips() { return numOfStrips; }
	unsigned int getNVertices() { return verticesPerStrip; }
};