#pragma once
#include "../Gen/HeightMapGen.h"
#include "../Gen/FractualNoiseGen.h"
#include <vector>
#include "../BufferManager.h"
#include "Heightmap.h"

class BaseTerrain {
protected:
	const float SIZE = 100.f;
	int terrainWidth;
	int terrainDepth;

	Heightmap* heightmap;
	BufferManager bm;
	
public:
	//bool LoadHeightMap(const char* mapPath);
	BaseTerrain(int width, int depth);
	~BaseTerrain(void)
	{};
	void InitializeTerrain();
	void Bind();
	void Draw();


};