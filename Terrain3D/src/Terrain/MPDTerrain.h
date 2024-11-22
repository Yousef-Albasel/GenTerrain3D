#pragma once
#include "../BufferManager.h"
#include "Heightmap.h"
#include "Texture.h"
// Terrain created using Midpoint Displacement Algorithm
class MPDTerrain
{
private:
    BufferManager bm;
    Heightmap* m_heightmap;
    std::vector<Texture*> m_textures;
    const float m_size;
    int terrainWidth;
    int terrainDepth;

    float m_minHeight = -1;
    float m_maxHeight = -1;

    void CreateMidPointDisplacementInternal(float roughness);

    void DiamondStep(int rectSize, float curHeight);
    void SquareStep(int rectSize, float curHeight);

public:
    MPDTerrain(int width, int depth, const float size);
    void CreateMidPointDisplacement(float roughness, float minHeight, float maxHeight);
    void InitializeTerrain();
    float getMaxHeightLoc() { return m_minHeight; };
    float getMinHeightLoc() { return m_maxHeight; };
    void Bind();
    void Draw() { bm.Draw(); };
};

