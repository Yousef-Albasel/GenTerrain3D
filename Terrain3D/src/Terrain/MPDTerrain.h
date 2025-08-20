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
    Texture* m_heightmapTexture;
    unsigned int m_heightmapTextureID;
    float m_minHeight = -1;
    float m_maxHeight = -1;

    void CreateMidPointDisplacementInternal(float roughness);

    void DiamondStep(int rectSize, float curHeight);
    void SquareStep(int rectSize, float curHeight);

public:
    MPDTerrain(int width, int depth, const float size);
    void CreateMidPointDisplacement(float roughness, float minHeight, float maxHeight);
    void CreateMidPointDisplacementTes(float roughness, float minHeight, float maxHeight);
    void CreateHeightmapTexture();
    void InitializeTerrain();
    float getMaxHeightLoc() { return m_maxHeight; };
    float getMinHeightLoc() { return m_minHeight; }; 

    unsigned int getHeightmapTextureID() { return m_heightmapTextureID; };
    void Bind();
    void Draw() { bm.Draw(); };
    void Draw_Patches() { bm.Draw_Patches(); };

};

