#include "patch.h"
#include <cmath>
#include <algorithm>

GeoMipPatch::GeoMipPatch(int x, int z, int size, Heightmap* heightmap)
    : m_startX(x), m_startZ(z), m_patchSize(size), m_currentLOD(0),
    m_heightmap(heightmap), m_buffersInitialized(false)
{
    m_maxLOD = static_cast<int>(log2(m_patchSize - 1));
    m_lodDistanceFactor = 50.0f;

    InitializeBuffers();
}

GeoMipPatch::~GeoMipPatch() {
}

void GeoMipPatch::InitializeBuffers() {
    if (m_buffersInitialized) return;

    // Extract the patch vertices from heightmap
    std::vector<float> patchVertices;
    std::vector<float> patchTexCoords;
    std::vector<glm::vec3> patchNormals;

    // Get vertices, texture coordinates, and normals for this patch
    for (int z = 0; z < m_patchSize; z++) {
        for (int x = 0; x < m_patchSize; x++) {
            int globalX = m_startX + x;
            int globalZ = m_startZ + z;

            // Ensure we don't go out of bounds
            if (globalX >= m_heightmap->getWidth() || globalZ >= m_heightmap->getDepth())
                continue;

            // Get vertex data from the heightmap
            float height = m_heightmap->GetHeightAt(globalX, globalZ);

            // Use the actual world position calculation from your heightmap
            float xPos = static_cast<float>(globalX) / (m_heightmap->getWidth() - 1) * m_heightmap->getSize();
            float zPos = static_cast<float>(globalZ) / (m_heightmap->getDepth() - 1) * m_heightmap->getSize();

            patchVertices.push_back(xPos);
            patchVertices.push_back(height);
            patchVertices.push_back(zPos);

            // Get texture coordinates
            patchTexCoords.push_back(static_cast<float>(globalX) / (m_heightmap->getWidth() - 1));
            patchTexCoords.push_back(static_cast<float>(globalZ) / (m_heightmap->getDepth() - 1));

            // Get normals
            int index = globalZ * m_heightmap->getWidth() + globalX;
            if (index < m_heightmap->GetNormals().size()) {
                patchNormals.push_back(m_heightmap->GetNormalsVec()[index]);
            }
            else {
                // Default normal if out of bounds
                patchNormals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
            }
        }
    }
    // Flatten normals 
    std::vector<float> flatNormals;
    for (const auto& normal : patchNormals) {
        flatNormals.push_back(normal.x);
        flatNormals.push_back(normal.y);
        flatNormals.push_back(normal.z);
    }

    GenerateIndices();

    // Initialize buffers
    m_bufferManager.InitializeBuffers(patchVertices, patchTexCoords, flatNormals, m_indices);
    m_buffersInitialized = true;
}

void GeoMipPatch::UpdateLOD(const glm::vec3& cameraPosition) {
    int previousLOD = m_currentLOD;
    CalculateLOD(cameraPosition);

    // If LOD changed, regenerate indices
    if (previousLOD != m_currentLOD) {
        GenerateIndices();
        m_buffersInitialized = false;
        InitializeBuffers();
    }
}

void GeoMipPatch::CalculateLOD(const glm::vec3& cameraPosition) {
    float distance = CalculatePatchDistance(cameraPosition);
    m_currentLOD = static_cast<int>(distance / m_lodDistanceFactor);
    m_currentLOD = std::max(0, std::min(m_currentLOD, m_maxLOD));
}

float GeoMipPatch::CalculatePatchDistance(const glm::vec3& cameraPosition) {
    // Calculate center of patch
    int centerX = m_startX + m_patchSize / 2;
    int centerZ = m_startZ + m_patchSize / 2;

    float xPos = static_cast<float>(centerX) / (m_heightmap->getWidth() - 1) * m_heightmap->getSize();
    float height = m_heightmap->GetHeightAt(centerX, centerZ);
    float zPos = static_cast<float>(centerZ) / (m_heightmap->getDepth() - 1) * m_heightmap->getSize();

    glm::vec3 patchCenter(xPos, height, zPos);

    // Calculate distance
    return glm::distance(cameraPosition, patchCenter);
}
void GeoMipPatch::GenerateIndices() {
    m_indices.clear();

    // Skip factor determined by LOD (2^LOD)
    int skip = 1 << m_currentLOD;

    // For debugging
    std::cout << "Generating indices for patch at (" << m_startX << ", " << m_startZ
        << ") with LOD " << m_currentLOD << " (skip=" << skip << ")" << std::endl;

    // Calculate actual vertices in each dimension after applying LOD
    int verticesPerSideX = (m_patchSize + skip - 1) / skip;
    int verticesPerSideZ = (m_patchSize + skip - 1) / skip;

    // Generate indices for triangles
    for (int z = 0; z < verticesPerSideZ - 1; z++) {
        for (int x = 0; x < verticesPerSideX - 1; x++) {
            // Calculate indices for the current quad at this LOD level
            int topLeft = (z * skip) * m_patchSize + (x * skip);
            int topRight = topLeft + skip;
            int bottomLeft = ((z + 1) * skip) * m_patchSize + (x * skip);
            int bottomRight = bottomLeft + skip;

            // First triangle
            m_indices.push_back(topLeft);
            m_indices.push_back(bottomLeft);
            m_indices.push_back(topRight);

            // Second triangle
            m_indices.push_back(topRight);
            m_indices.push_back(bottomLeft);
            m_indices.push_back(bottomRight);
        }
    }

    std::cout << "Generated " << m_indices.size() / 3 << " triangles" << std::endl;
}
void GeoMipPatch::BindBuffers() {
    m_bufferManager.Bind();
}

void GeoMipPatch::Draw() const {
    m_bufferManager.Draw();
}