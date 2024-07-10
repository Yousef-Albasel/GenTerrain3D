#pragma once
#include "GL/glew.h"
#include "../BufferManager.h"
#include "Heightmap.h"
class FaultFormationTerrain {
public:
    FaultFormationTerrain(int width, int depth, const float size);
    void InitializeTerrain();
    void CreateFaultFormation(int TerrainSize, int iteration, float minHeight, float maxHeight,float filter);
    
    void Bind() { bm.Bind(); };
    void Draw() { bm.Draw(); };
    
    void setMinMaxHeight(float min, float max) { m_maxHeightLoc = max, m_minHeightLoc = min; };
    float getMaxHeightLoc() { return m_maxHeightLoc; };
    float getMinHeightLoc() { return m_minHeightLoc; };

private:
    BufferManager bm;
    Heightmap* m_heightmap;
    const float m_size;
    int terrainWidth;
    int terrainDepth;


    unsigned int m_minHeightLoc = -1;
    unsigned int m_maxHeightLoc = -1;

    struct TerrainPoint {
        int x = 0;
        int z = 0;
        void Print() {
            printf("[%d,%d]", x, z);
        }
        bool isEqual(TerrainPoint& p) const {
            return ((x == p.x) && (z == p.z));
        }
    };

    void  CreateFaultFormationInternal(int Iteration, float minHeight, float maxHeight,float filter);
    void  GenRandomTerrainPoints(TerrainPoint& p1, TerrainPoint& p2);
    void  InitializeBuffers();

};
