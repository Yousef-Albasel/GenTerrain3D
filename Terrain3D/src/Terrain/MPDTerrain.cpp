#include "MPDTerrain.h"

MPDTerrain::MPDTerrain(int width, int depth, const float size) : terrainWidth(width), terrainDepth(depth), m_size(size)
{
    m_heightmapTexture = nullptr;
    m_heightmap = new Heightmap(size, width, depth);
    for (size_t i = 0; i < m_textures.size(); ++i)
        m_textures[i] = nullptr;
}



void MPDTerrain::InitializeTerrain()
{
    m_heightmap->InitializeHeightMap();

    // Add textures for different height ranges
    m_textures.push_back(new Texture("Textures/low.jpg")); // Low height (grass)
    m_textures.push_back(new Texture("Textures/optimal.jpg"));  // Medium-low height (dirt)
    m_textures.push_back(new Texture("Textures/high.jpg"));  // Medium-high height (rock)
    m_textures.push_back(new Texture("Textures/top.jpg"));  // High height (snow)
}

void MPDTerrain::Bind()
{
    bm.Bind();
    for (size_t i = 0; i < m_textures.size(); ++i)
    {
        m_textures[i]->Bind(static_cast<unsigned int>(i)); // Bind each texture to a texture unit
    }
}


void MPDTerrain::CreateMidPointDisplacement(float roughness, float minHeight, float maxHeight)
{

    if (roughness < 0.0f) {
        printf("%s: roughness must be positive - %f\n", __FUNCTION__, roughness);
        exit(0);
    }

    m_minHeight = minHeight;
    m_maxHeight = maxHeight;
    float heightScale = m_size / m_heightmap->getSize();
    CreateMidPointDisplacementInternal(roughness);
    m_heightmap->NormalizeHeights(maxHeight * heightScale, minHeight);

    m_heightmap->CalculateNormals();
    bm.InitializeBuffers(m_heightmap->GetVertices(), m_heightmap->GetTexCoord(),m_heightmap->GetNormals(), m_heightmap->GetIndices());

}
void MPDTerrain::CreateMidPointDisplacementTes(float roughness, float minHeight, float maxHeight)
{
    if (roughness < 0.0f) {
        printf("%s: roughness must be positive - %f\n", __FUNCTION__, roughness);
        exit(0);
    }

    std::cout << "=== Creating tessellation terrain ===" << std::endl;
    std::cout << "Input parameters:" << std::endl;
    std::cout << "  Roughness: " << roughness << std::endl;
    std::cout << "  Min Height: " << minHeight << std::endl;
    std::cout << "  Max Height: " << maxHeight << std::endl;
    std::cout << "  Terrain dimensions: " << terrainWidth << "x" << terrainDepth << std::endl;

    m_minHeight = minHeight;
    m_maxHeight = maxHeight;

    // Generate the heightmap data first
    std::cout << "Generating midpoint displacement..." << std::endl;
    CreateMidPointDisplacementInternal(roughness);

    // Check heights before normalization
    float minBefore, maxBefore;
    m_heightmap->GetMinMax(minBefore, maxBefore);
    std::cout << "Heights before normalization: min=" << minBefore << ", max=" << maxBefore << std::endl;

    std::cout << "Normalizing heights..." << std::endl;
    m_heightmap->NormalizeHeights(maxHeight, minHeight);

    // Check heights after normalization
    float minAfter, maxAfter;
    m_heightmap->GetMinMax(minAfter, maxAfter);
    std::cout << "Heights after normalization: min=" << minAfter << ", max=" << maxAfter << std::endl;

    // Sample a few height values for debugging
    std::cout << "Sample height values:" << std::endl;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (i * 64 < terrainWidth && j * 64 < terrainDepth) {
                float h = m_heightmap->GetHeightAt(i * 64, j * 64);
                std::cout << "  Height[" << i * 64 << "," << j * 64 << "] = " << h << std::endl;
            }
        }
    }

    // Create the heightmap texture BEFORE switching to quad patches
    std::cout << "Creating heightmap texture..." << std::endl;
    CreateHeightmapTexture();

    // Now switch to quad patch geometry for tessellation
    std::cout << "Generating quad patches..." << std::endl;
    if (!m_heightmap->GenerateQuadPatches()) {
        std::cout << "ERROR: Failed to generate quad patches!" << std::endl;
        return;
    }

    // For tessellation, we don't need complex normals since they're calculated in TES
    std::cout << "Setting up simple normals for tessellation..." << std::endl;
    m_heightmap->CalculateNormals(); // This will now use the simple version

    // Get the data for buffer initialization
    std::vector<float> vertices = m_heightmap->GetVertices();
    std::vector<float> texCoords = m_heightmap->GetTexCoord();
    std::vector<float> normals = m_heightmap->GetNormals();
    std::vector<unsigned int> indices = m_heightmap->GetIndices();

    std::cout << "Data sizes before buffer creation:" << std::endl;
    std::cout << "  Vertices: " << vertices.size() << " (" << vertices.size() / 3 << " vertices)" << std::endl;
    std::cout << "  TexCoords: " << texCoords.size() << " (" << texCoords.size() / 2 << " coords)" << std::endl;
    std::cout << "  Normals: " << normals.size() << " (" << normals.size() / 3 << " normals)" << std::endl;
    std::cout << "  Indices: " << indices.size() << " (" << indices.size() / 4 << " patches)" << std::endl;

    // Sample vertex positions for debugging
    std::cout << "Sample vertex positions (first 5):" << std::endl;
    for (int i = 0; i < 5 && i * 3 + 2 < vertices.size(); i++) {
        std::cout << "  Vertex[" << i << "] = ("
            << vertices[i * 3] << ", "
            << vertices[i * 3 + 1] << ", "
            << vertices[i * 3 + 2] << ")" << std::endl;
    }

    // Validate data integrity
    if (vertices.size() / 3 != texCoords.size() / 2 ||
        vertices.size() != normals.size()) {
        std::cout << "ERROR: Data size mismatch!" << std::endl;
        return;
    }

    // Initialize buffers with the new geometry
    std::cout << "Initializing buffers..." << std::endl;
    try {
        bm.InitializeBuffers(vertices, texCoords, normals, indices);
        std::cout << "Tessellation terrain created successfully!" << std::endl;
        std::cout << "Final terrain parameters:" << std::endl;
        std::cout << "  m_minHeight: " << m_minHeight << std::endl;
        std::cout << "  m_maxHeight: " << m_maxHeight << std::endl;
        std::cout << "  getMinHeightLoc(): " << getMinHeightLoc() << std::endl;
        std::cout << "  getMaxHeightLoc(): " << getMaxHeightLoc() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "ERROR: Exception during buffer initialization: " << e.what() << std::endl;
    }
}

void MPDTerrain::CreateHeightmapTexture() {
    int width = m_heightmap->getWidth();
    int height = m_heightmap->getDepth();

    std::vector<float> heightData(width * height);

    float minH, maxH;
    m_heightmap->GetMinMax(minH, maxH);
    float heightRange = maxH - minH;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float normalizedHeight = (m_heightmap->GetHeightAt(j, i) - minH) / heightRange;
            heightData[i * width + j] = normalizedHeight;
        }
    }

    glGenTextures(1, &m_heightmapTextureID);
    glBindTexture(GL_TEXTURE_2D, m_heightmapTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, heightData.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout << "Heightmap texture created: " << width << "x" << height << std::endl;
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


