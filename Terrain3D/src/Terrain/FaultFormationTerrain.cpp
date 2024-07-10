#include "FaultFormationTerrain.h"

#include <iostream>


FaultFormationTerrain::FaultFormationTerrain(int width, int depth, const float size) : terrainWidth(width), terrainDepth(depth),m_size(size)
    {
    m_heightmap = new Heightmap(size, width, depth);
    }

void FaultFormationTerrain::InitializeTerrain()
{
    m_heightmap->InitializeHeightMap();
}

void FaultFormationTerrain::CreateFaultFormation(int TerrainSize, int iteration, float minHeight, float maxHeight,float filter) {
    CreateFaultFormationInternal(iteration, minHeight, maxHeight,filter);
    setMinMaxHeight(minHeight, maxHeight);
    m_heightmap->NormalizeHeights(maxHeight,minHeight);
    InitializeBuffers();

}

void FaultFormationTerrain::CreateFaultFormationInternal(int Iteration, float minHeight, float maxHeight,float filter) {


    float DeltaHeight = maxHeight - minHeight;
    for (int CurIter = 0; CurIter < Iteration; CurIter++) {
        float IterationRatio = ((float)CurIter / (float)Iteration);
        float Height = maxHeight - IterationRatio * DeltaHeight;

        TerrainPoint p1, p2;
        GenRandomTerrainPoints(p1, p2); // getting two random points on the terrain.

        int DirX = p2.x - p1.x;
        int DirZ = p2.z - p1.z; 

        // looping and incrementing the height in one side of the terrain
        for (int z = 0; z < terrainDepth; z++) {
            for (int x = 0; x < terrainWidth; x++) {
                int DirX_in = x - p1.x;
                int DirZ_in = z - p1.z;

                int CrossProduct = DirX_in * DirZ - DirX * DirZ_in;

                if (CrossProduct > 0) {
                    // Increase The height of ur original grid
                    float currentHeight = m_heightmap->GetHeightAt(x, z);
                    m_heightmap->SetHeightAt(x, z, currentHeight + Height);
                }
            }
        }

    
    }

    m_heightmap->ApplyFIRFilter(filter);
}

void FaultFormationTerrain::GenRandomTerrainPoints(TerrainPoint& p1, TerrainPoint& p2) {
    p1.x = rand() % terrainWidth;
    p1.z = rand() % terrainDepth;

    int Counter = 0;
    do {
        p2.x = rand() % terrainWidth;
        p2.z = rand() % terrainDepth;

        if (Counter++ == 1000) {
            std::cout << "Infinite Loop Detected\n";
            std::cout << "p1: (" << p1.x << ", " << p1.z << ")\n";
            p2.x = (p1.x + 1) % terrainWidth;
            p2.z = (p1.z + 1) % terrainDepth;
            std::cout << "p2 set to: (" << p2.x << ", " << p2.z << ")\n";
            break;
        }
    } while (p1.isEqual(p2));
}


void FaultFormationTerrain::InitializeBuffers() {
    bm.InitializeBuffers(m_heightmap->GetVertices(), m_heightmap->GetIndices());
}
