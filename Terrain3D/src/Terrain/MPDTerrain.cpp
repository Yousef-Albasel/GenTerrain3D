#include "MPDTerrain.h"

MPDTerrain::MPDTerrain(int width, int depth, const float size) : terrainWidth(width), terrainDepth(depth), m_size(size)
{
    m_heightmap = new Heightmap(size, width, depth);
}



void MPDTerrain::InitializeTerrain()
{
    m_heightmap->InitializeHeightMap();
}

void MPDTerrain::CreateMidPointDisplacement(float roughness, float minHeight, float maxHeight)
{

    if (roughness < 0.0f) {
        printf("%s: roughness must be positive - %f\n", __FUNCTION__, roughness);
        exit(0);
    }

    m_minHeight = minHeight;
    m_maxHeight = maxHeight;
    CreateMidPointDisplacementInternal(roughness);
    m_heightmap->NormalizeHeights(maxHeight, minHeight);
    bm.InitializeBuffers(m_heightmap->GetVertices(), m_heightmap->GetIndices());

}
void MPDTerrain::CreateMidPointDisplacementInternal(float roughness)
{
    int rectSize = terrainWidth;
    float curHeight = (float)rectSize / 2.0f;
    float heightReduce = pow(2.0f, -roughness);

    while (rectSize > 0) {
        DiamondStep(rectSize, curHeight);
        SquareStep(rectSize, curHeight);
        rectSize /= 2;
        curHeight *= heightReduce;
    }
}

void MPDTerrain::DiamondStep(int rectSize, float curHeight) {
    int halfRectSize = rectSize / 2;
    for (int x = 0; x < terrainWidth; x += rectSize) {
        for (int z = 0; z < terrainDepth; z += rectSize) {
            // Those two represents corners
            int next_x = (x + rectSize) % terrainWidth;
            int next_z = (z + rectSize) % terrainDepth;
            
            float TL = m_heightmap->GetHeightAt(x, z);
            float TR = m_heightmap->GetHeightAt(next_x, z);
            float BL = m_heightmap->GetHeightAt(x, next_z);
            float BR = m_heightmap->GetHeightAt(next_x,next_z);
        
            int mid_x = x + halfRectSize;
            int mid_z = z + halfRectSize;
            
            float RandValue = m_heightmap->RandomFloatRange(curHeight, -curHeight);
            float MidPoint = (TL + TR + BL + BR) / 4.0f;

            m_heightmap->SetHeightAt(mid_x, mid_z, MidPoint + RandValue);
        }
    }

};

void MPDTerrain::SquareStep(int rectSize, float curHeight) {
    int halfRectSize = rectSize / 2;
    for (int x = 0; x < terrainWidth; x += rectSize) {
        for (int z = 0; z < terrainDepth; z += rectSize) {
            int next_x = (x + rectSize) % terrainWidth;
            int next_z = (z + rectSize) % terrainDepth;
            
            int mid_x = x + halfRectSize;
            int mid_z = z + halfRectSize;

            int prev_mid_x = (x - halfRectSize + terrainWidth) % terrainWidth;
            int prev_mid_z= (z - halfRectSize + terrainDepth) % terrainDepth;

            float curTL = m_heightmap->GetHeightAt(x, z);
            float curTR = m_heightmap->GetHeightAt(next_x, z);
            float curCenter = m_heightmap->GetHeightAt(mid_x, mid_z);
            float prevZCenter = m_heightmap->GetHeightAt(mid_x, prev_mid_z);
            float curBL = m_heightmap->GetHeightAt(x, next_z);
            float prevXCenter = m_heightmap->GetHeightAt(prev_mid_x, mid_z);

            float curLeftMid = (curTL + curCenter + curBL + prevXCenter) / 4.0f + m_heightmap->RandomFloatRange(-curHeight, curHeight);
            float curTopMid = (curTL + curCenter + curBL + prevZCenter) / 4.0f + m_heightmap->RandomFloatRange(-curHeight, curHeight);

            m_heightmap->SetHeightAt(mid_x, z, curTopMid);
            m_heightmap->SetHeightAt(x, mid_z, curLeftMid);
        }
    }
};