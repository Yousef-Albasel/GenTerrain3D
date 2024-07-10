#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "BaseTerrain.h"
#include <iostream>

BaseTerrain::BaseTerrain(int width, int depth) : terrainWidth(width), terrainDepth(depth)
{
    heightmap = new Heightmap(SIZE, width, depth);
}

void BaseTerrain::InitializeTerrain()
{
    heightmap->InitializeHeightMap();

    bm.InitializeBuffers(heightmap->GetVertices(), heightmap->GetIndices());
}


void BaseTerrain::Bind() {
    bm.Bind();
}

void BaseTerrain::Draw() {
    bm.Draw();
}