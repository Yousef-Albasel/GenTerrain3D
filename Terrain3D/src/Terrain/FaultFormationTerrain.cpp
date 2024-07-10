#include "FaultFormationTerrain.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

float clamp(float d, float min, float max) {
    const float t = d < min ? min : d;
    return t > max ? max : t;
}

void FaultFormationTerrain::PrintVertices() {
 int j = 0;
for (auto i = 0;i < vertices.size(); i++)
{
    std::cout << vertices[i] << ", ";
    j++;
    if (j > 2)
    {
        std::cout << std::endl;
        j = 0;
    }
}
}

FaultFormationTerrain::FaultFormationTerrain(int width, int depth) : terrainWidth(width), terrainDepth(depth)
    {
    vertexCount = terrainWidth * terrainDepth;
    vertices.resize(vertexCount * 3);
    indices.resize((terrainWidth - 1) * (terrainDepth - 1) * 6);
    srand(time(0));
    }




void FaultFormationTerrain::InitializeTerrain()
{

}

bool FaultFormationTerrain::GenerateVertices()
{
    int idx = 0;
    for (int i = 0; i < terrainDepth; ++i) {
        for (int j = 0; j < terrainWidth; ++j) {
            vertices[idx * 3] = static_cast<float>(j) / (terrainWidth - 1) * SIZE;
            vertices[idx * 3 + 1] = 0.f;
            vertices[idx * 3 + 2] = static_cast<float>(i) / (terrainDepth - 1) * SIZE;
            idx++;
        }
    }

    int pointer = 0;
    for (int gz = 0; gz < terrainDepth - 1; ++gz) {
        for (int gx = 0; gx < terrainWidth - 1; ++gx) {

            int topLeft = (gz * terrainWidth) + gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz + 1) * terrainWidth) + gx;
            int bottomRight = bottomLeft + 1;

            indices[pointer++] = topLeft;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = topRight;
            indices[pointer++] = topRight;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = bottomRight;
        }
    }

    return true;
}


void FaultFormationTerrain::CreateFaultFormation(int TerrainSize, int iteration, float minHeight, float maxHeight,float filter) {
    GenerateVertices();
    CreateFaultFormationInternal(iteration, minHeight, maxHeight,filter);

    setMinMaxHeight(minHeight, maxHeight);

    NormalizeHeights(maxHeight,minHeight);

  
    // create the buffers
    InitializeBuffers();

}

void FaultFormationTerrain::CreateFaultFormationInternal(int Iteration, float minHeight, float maxHeight,float filter) {
    std::cout << terrainDepth << ":" << terrainWidth;

    if (vertices.empty()) {
        std::cerr << "Error No vertices\n";
        return;
    }

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
                    float currentHeight = GetHeightAt(x, z);
                    SetHeightAt(x, z, currentHeight + Height);
                }
            }
        }

    
    }

    ApplyFIRFilter(filter);
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


void FaultFormationTerrain::NormalizeHeights(float MaxRange,float MinRange) {
    float Min, Max;

    GetMinMax(Min, Max);

    if (Max <= Min) {
        return;
    }

    float MinMaxDelta = Max - Min;
    float MinMaxRange = MaxRange - MinRange;

    for (int i = 0; i < terrainDepth * terrainWidth; i++) {
        vertices[i * 3 + 1] = ((vertices[i * 3 + 1] - Min) / MinMaxDelta) * MinMaxRange + MinRange;
    }


}

void FaultFormationTerrain::GetMinMax(float& Min, float& Max) {
    Max = Min = vertices[1]; 

    for (int i = 0; i < vertexCount; ++i) {
        float y = vertices[i * 3 + 1];
        if (y < Min) {
            Min = y;
        }
        if (y > Max) {
            Max = y;
        }
    }
}
 
float FaultFormationTerrain::GetHeightAt(int x, int z) const {
    if (x < 0 || x >= terrainWidth || z < 0 || z >= terrainDepth) {
        std::cerr << "Error: Coordinates out of bounds." << std::endl;
        return 0.0f;
    }
    int index = (z * terrainWidth + x) * 3 + 1; // y-coordinate is at offset 1
    return vertices[index];
}

void FaultFormationTerrain::SetHeightAt(int x, int z, float height) {
    if (x < 0 || x >= terrainWidth || z < 0 || z >= terrainDepth) {
        std::cerr << "Error: Coordinates out of bounds." << std::endl;
        return;
    }
    int index = (z * terrainWidth + x) * 3 + 1; // y-coordinate is at offset 1
    vertices[index] = height;

}

void FaultFormationTerrain::InitializeBuffers() {
    bm.InitializeBuffers(vertices, indices);
}

void FaultFormationTerrain::ApplyFIRFilter(float Filter)
{
    // left to right
    for (int z = 0; z < terrainWidth; z++) {
        float PrevVal = GetHeightAt(0, z);
        for (int x = 1; x < terrainDepth; x++) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }

    // right to left
    for (int z = 0; z < terrainWidth; z++) {
        float PrevVal = GetHeightAt(terrainDepth - 1, z);
        for (int x = terrainDepth - 2; x >= 0; x--) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }

    // bottom to top
    for (int x = 0; x < terrainDepth; x++) {
        float PrevVal = GetHeightAt(x, 0);
        for (int z = 1; z < terrainWidth; z++) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }

    // top to bottom
    for (int x = 0; x < terrainDepth; x++) {
        float PrevVal = GetHeightAt(x, terrainDepth - 1);
        for (int z = terrainWidth - 2; z >= 0; z--) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }
}


float FaultFormationTerrain::FIRFilterSinglePoint(int x, int z, float PrevVal, float Filter)
{
    float CurVal = GetHeightAt(x, z);
    float NewVal = Filter * PrevVal + (1 - Filter) * CurVal;
    SetHeightAt(x, z, NewVal);
    return NewVal;
}

