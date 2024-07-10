#pragma once
#include "GL/glew.h"
#include "../BufferManager.h"
//#include "BaseTerrain.h"
class FaultFormationTerrain {
public:
    FaultFormationTerrain(int width, int depth);
    void InitializeTerrain();
    bool GenerateVertices();
    
    void CreateFaultFormation(int TerrainSize, int iteration, float minHeight, float maxHeight,float filter);
    
    void Bind() { bm.Bind(); };
    void Draw() { bm.Draw(); };

    float getSize() { return SIZE; }
    float GetHeightAt(int x, int z) const;
    void SetHeightAt(int x, int z, float height);

    void setMinMaxHeight(float min, float max) { m_maxHeightLoc = max, m_minHeightLoc = min; };
    float getMaxHeightLoc() { return m_maxHeightLoc; };
    float getMinHeightLoc() { return m_minHeightLoc; };
private:
    
    BufferManager bm;
    const float SIZE = 100.f;
    int terrainWidth;
    int terrainDepth;

    int vertexCount;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    //unsigned int VAO;  // VAO ID
    //unsigned int VBO;  // VBO ID
    //unsigned int EBO;  // EBO ID

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
    void  NormalizeHeights(float MaxRange, float MinRange);
    void  InitializeBuffers();
    void  GetMinMax(float& Min, float& Max);
    void  ApplyFIRFilter(float filter);
    float FIRFilterSinglePoint(int x, int z, float PrevVal, float Filter);
    void  PrintVertices();
};
