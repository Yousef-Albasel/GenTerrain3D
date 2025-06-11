// GeoMipPatch.h
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Heightmap.h"
#include "../BufferManager.h"

class GeoMipPatch {
public:
    GeoMipPatch(int x, int z, int size, Heightmap* heightmap);
    ~GeoMipPatch();

    void UpdateLOD(const glm::vec3& cameraPosition);
    void GenerateIndices();
    void BindBuffers();
    void Draw() const;

private:
    int m_startX, m_startZ;              // Starting position in the heightmap
    int m_patchSize;                     // Size of this patch (in vertices)
    int m_currentLOD;                    // Current level of detail (0 = max detail)
    int m_maxLOD;                        // Maximum level of detail
    float m_lodDistanceFactor;           // Distance factor for LOD calculations
    Heightmap* m_heightmap;              // Reference to the heightmap
    BufferManager m_bufferManager;       // Buffer manager for this patch
    std::vector<unsigned int> m_indices; // Indices for different LOD levels
    bool m_buffersInitialized;

    void CalculateLOD(const glm::vec3& cameraPosition);
    float CalculatePatchDistance(const glm::vec3& cameraPosition);
    void InitializeBuffers();
};