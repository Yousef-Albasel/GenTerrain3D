#include "Heightmap.h"
// Constructor

Heightmap::Heightmap(float size, int width, int depth)
    : m_size(size), m_width(width), m_depth(depth) {
    vertexCount = m_width * m_depth;
    vertices.resize(vertexCount * 3);
    indices.resize((m_width - 1) * (m_depth - 1) * 6);
    texCoords.resize(vertexCount * 2); // Texture Coordinates are a 2D Vector
    srand(static_cast<unsigned int>(time(0)));
    m_rez = 32;
}

Heightmap::Heightmap(float size, int width, int depth, int rez = 32)
    : m_size(size), m_width(width), m_depth(depth), m_rez(rez) {
    vertexCount = m_width * m_depth;
    vertices.resize(vertexCount * 3);
    indices.resize((m_width - 1) * (m_depth - 1) * 6);
    texCoords.resize(vertexCount * 2); // Texture Coordinates are a 2D Vector
    srand(static_cast<unsigned int>(time(0)));
}

// Initialize Height Map
void Heightmap::InitializeHeightMap() {
    GenerateVertices();
    GenerateTextureCoords();
}

bool Heightmap::GenerateVertices()
{
    int idx = 0;
    for (int i = 0; i < m_depth; ++i) {
        for (int j = 0; j < m_width; ++j) {
            vertices[idx * 3] = static_cast<float>(j) / (m_width - 1) * m_size;
            vertices[idx * 3 + 1] = 0.f;
            vertices[idx * 3 + 2] = static_cast<float>(i) / (m_depth - 1) * m_size;
            idx++;
        }
    }

    int pointer = 0;
    for (int gz = 0; gz < m_depth - 1; ++gz) {
        for (int gx = 0; gx < m_width - 1; ++gx) {

            int topLeft = (gz * m_width) + gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz + 1) * m_width) + gx;
            int bottomRight = bottomLeft + 1;

            indices[pointer++] = topLeft;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = topRight;
            indices[pointer++] = topRight;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = bottomRight;
        }
    }
    std::cout << "Vertices Generated" << std::endl;
    return true;
}

void Heightmap::GenerateTextureCoords() {
    int idx = 0;
    for (int i = 0; i < m_depth; ++i) {
        for (int j = 0; j < m_width; ++j) {
            texCoords[idx * 2] = static_cast<float>(j) / (m_width - 1);
            texCoords[idx * 2 + 1] = static_cast<float>(i) / (m_depth - 1);
            idx++;
        }
    }
    std::cout << "Texture Coords Generated" << std::endl;

}

//void Heightmap::CalculateNormals() {
//    normals.resize(vertexCount, glm::vec3(0.0f, 0.0f, 0.0f));
//
//    for (size_t i = 0; i < indices.size(); i += 3) {
//        int idx0 = indices[i];
//        int idx1 = indices[i + 1];
//        int idx2 = indices[i + 2];
//
//        glm::vec3 v0 = glm::vec3(vertices[idx0 * 3], vertices[idx0 * 3 + 1], vertices[idx0 * 3 + 2]);
//        glm::vec3 v1 = glm::vec3(vertices[idx1 * 3], vertices[idx1 * 3 + 1], vertices[idx1 * 3 + 2]);
//        glm::vec3 v2 = glm::vec3(vertices[idx2 * 3], vertices[idx2 * 3 + 1], vertices[idx2 * 3 + 2]);
//
//        glm::vec3 edge1 = v1 - v0;
//        glm::vec3 edge2 = v2 - v0;
//        glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));
//        
//        normals[idx0] += faceNormal;
//        normals[idx1] += faceNormal;
//        normals[idx2] += faceNormal;
//    }
//
//    // Normalize all vertex normals
//    for (auto& normal : normals) {
//        normal = glm::normalize(normal);
//    }
//    DebugNormals();
//    std::cout << "Normal Generated" << std::endl;
//
//}
void Heightmap::DebugNormals() {
    for (size_t i = 0; i < 5; i++) {
        std::cout << "Vertex " << i << " position: "
            << vertices[i * 3] << ", "
            << vertices[i * 3 + 1] << ", "
            << vertices[i * 3 + 2] << std::endl;
        std::cout << "Normal " << i << ": "
            << normals[i].x << ", "
            << normals[i].y << ", "
            << normals[i].z << std::endl;
    }
}

void Heightmap::NormalizeHeights(float MaxRange, float MinRange) {
    float Min, Max;

    GetMinMax(Min, Max);

    if (Max <= Min) {
        return;
    }

    float MinMaxDelta = Max - Min;
    float MinMaxRange = MaxRange - MinRange;

    for (int i = 0; i < m_depth * m_width; i++) {
        vertices[i * 3 + 1] = ((vertices[i * 3 + 1] - Min) / MinMaxDelta) * MinMaxRange + MinRange;
    }


}

void Heightmap::GetMinMax(float& Min, float& Max) {
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

float Heightmap::GetHeightAt(int x, int z) const {
    if (x < 0 || x >= m_width || z < 0 || z >= m_depth) {
        printf("Error: Coordinates out of bounds.\n");
        return 0.0f;
    }
    int index = (z * m_width + x) * 3 + 1; // y-coordinate is at offset 1
    return vertices[index];
}

void Heightmap::SetHeightAt(int x, int z, float height) {
    if (x < 0 || x >= m_width || z < 0 || z >= m_depth) {
        printf("Error: Coordinates out of bounds.\n");
        return;
    }
    int index = (z * m_width + x) * 3 + 1; // y-coordinate is at offset 1
    vertices[index] = height;

}


void Heightmap::ApplyFIRFilter(float Filter)
{
    // left to right
    for (int z = 0; z < m_width; z++) {
        float PrevVal = GetHeightAt(0, z);
        for (int x = 1; x < m_depth; x++) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }

    // right to left
    for (int z = 0; z < m_width; z++) {
        float PrevVal = GetHeightAt(m_depth - 1, z);
        for (int x = m_depth - 2; x >= 0; x--) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }

    // bottom to top
    for (int x = 0; x < m_depth; x++) {
        float PrevVal = GetHeightAt(x, 0);
        for (int z = 1; z < m_width; z++) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }

    // top to bottom
    for (int x = 0; x < m_depth; x++) {
        float PrevVal = GetHeightAt(x, m_depth - 1);
        for (int z = m_width - 2; z >= 0; z--) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }
}


float Heightmap::FIRFilterSinglePoint(int x, int z, float PrevVal, float Filter)
{
    float CurVal = GetHeightAt(x, z);
    float NewVal = Filter * PrevVal + (1 - Filter) * CurVal;
    SetHeightAt(x, z, NewVal);
    return NewVal;
}

void Heightmap::PrintVertices() {
    int j = 0;
    for (auto i = 0;i < vertices.size(); i++)
    {
        printf("%f, ",vertices[i]);
        j++;
        if (j > 2)
        {
            printf("\n");
            j = 0;
        }
    }
}


float Heightmap::RandomFloatRange(float Start, float End)
{
    if (End == Start) {
        printf("Invalid random range: (%f, %f)\n", Start, End);
        exit(0);
    }

    float Delta = End - Start;

    float RandomValue = RandomFloat() * Delta + Start;

    return RandomValue;
}

float Heightmap::RandomFloat()
{
    float Max = RAND_MAX;
    return ((float)rand() / Max);
}

// =================== Tesselation Code ===================== //
bool Heightmap::GenerateQuadPatches() {
    // Calculate vertices needed: (rez+1) x (rez+1) grid for rez x rez patches
    int patchVertexCount = (m_rez + 1) * (m_rez + 1);

    // Clear and resize arrays safely
    vertices.clear();
    texCoords.clear();
    indices.clear();
    normals.clear();

    vertices.resize(patchVertexCount * 3);
    texCoords.resize(patchVertexCount * 2);
    normals.resize(patchVertexCount, glm::vec3(0.0f, 1.0f, 0.0f));

    // Generate vertices for quad patches
    int idx = 0;
    for (int i = 0; i <= m_rez; ++i) {
        for (int j = 0; j <= m_rez; ++j) {
            // Ensure we don't exceed array bounds
            if (idx >= patchVertexCount) {
                std::cout << "ERROR: Vertex index out of bounds!" << std::endl;
                return false;
            }

            // World space position: span from -size/2 to +size/2
            vertices[idx * 3 + 0] = (static_cast<float>(j) / m_rez - 0.5f) * m_size;     // X
            vertices[idx * 3 + 1] = 0.0f;                                                // Y
            vertices[idx * 3 + 2] = (static_cast<float>(i) / m_rez - 0.5f) * m_size;     // Z

            // Texture coordinates: span 0 to 1 across resolution blocks
            texCoords[idx * 2 + 0] = static_cast<float>(j) / m_rez;     // U
            texCoords[idx * 2 + 1] = static_cast<float>(i) / m_rez;     // V

            idx++;
        }
    }

    // Generate indices for quad patches (for use with GL_PATCHES)
    int numPatches = m_rez * m_rez;
    indices.resize(numPatches * 4); // 4 vertices per quad patch
    int indexPointer = 0;

    for (int i = 0; i < m_rez; ++i) {
        for (int j = 0; j < m_rez; ++j) {
            int topLeft = i * (m_rez + 1) + j;
            int topRight = topLeft + 1;
            int bottomLeft = (i + 1) * (m_rez + 1) + j;
            int bottomRight = bottomLeft + 1;

            // Bounds checking for indices
            if (topLeft >= patchVertexCount || topRight >= patchVertexCount ||
                bottomLeft >= patchVertexCount || bottomRight >= patchVertexCount) {
                std::cout << "ERROR: Index out of bounds!" << std::endl;
                std::cout << "Patch (" << i << ", " << j << "): "
                    << topLeft << ", " << topRight << ", "
                    << bottomLeft << ", " << bottomRight
                    << " (max: " << patchVertexCount << ")" << std::endl;
                return false;
            }

            // Each patch is a quad (4 vertices) - order matters for tessellation
            indices[indexPointer++] = topLeft;
            indices[indexPointer++] = topRight;
            indices[indexPointer++] = bottomRight;
            indices[indexPointer++] = bottomLeft;
        }
    }

    // Update vertex count for the new geometry
    vertexCount = patchVertexCount;

    std::cout << "Quad patches generated successfully: " << m_rez << "x" << m_rez
        << " (" << vertexCount << " vertices, " << numPatches << " patches)" << std::endl;
    std::cout << "Arrays - Vertices: " << vertices.size() << ", TexCoords: " << texCoords.size()
        << ", Indices: " << indices.size() << ", Normals: " << normals.size() << std::endl;

    return true;
}
void Heightmap::CalculateNormals() {
    // For tessellation, we don't need complex normal calculation since
    // normals will be computed in the tessellation evaluation shader
    // Just provide simple upward-pointing normals

    normals.clear();
    normals.resize(vertexCount, glm::vec3(0.0f, 1.0f, 0.0f));

    std::cout << "Simple normals generated for tessellation: " << normals.size() << " normals" << std::endl;
}

// Alternative: If you want proper normal calculation for quad patches
void Heightmap::CalculateNormalsForQuadPatches() {
    normals.clear();
    normals.resize(vertexCount, glm::vec3(0.0f, 0.0f, 0.0f));

    // Process each quad patch (4 vertices per patch)
    for (size_t i = 0; i < indices.size(); i += 4) {
        // Each quad has 4 vertices
        unsigned int idx0 = indices[i];     // Top-left
        unsigned int idx1 = indices[i + 1]; // Top-right
        unsigned int idx2 = indices[i + 2]; // Bottom-right
        unsigned int idx3 = indices[i + 3]; // Bottom-left

        // Bounds checking
        if (idx0 >= vertexCount || idx1 >= vertexCount ||
            idx2 >= vertexCount || idx3 >= vertexCount) {
            std::cout << "ERROR: Index out of bounds in normal calculation!" << std::endl;
            continue;
        }

        // Get vertex positions
        glm::vec3 v0(vertices[idx0 * 3], vertices[idx0 * 3 + 1], vertices[idx0 * 3 + 2]);
        glm::vec3 v1(vertices[idx1 * 3], vertices[idx1 * 3 + 1], vertices[idx1 * 3 + 2]);
        glm::vec3 v2(vertices[idx2 * 3], vertices[idx2 * 3 + 1], vertices[idx2 * 3 + 2]);
        glm::vec3 v3(vertices[idx3 * 3], vertices[idx3 * 3 + 1], vertices[idx3 * 3 + 2]);

        // Calculate two triangles from the quad
        // Triangle 1: v0, v1, v2
        glm::vec3 edge1_1 = v1 - v0;
        glm::vec3 edge2_1 = v2 - v0;
        glm::vec3 normal1 = glm::normalize(glm::cross(edge1_1, edge2_1));

        // Triangle 2: v0, v2, v3
        glm::vec3 edge1_2 = v2 - v0;
        glm::vec3 edge2_2 = v3 - v0;
        glm::vec3 normal2 = glm::normalize(glm::cross(edge1_2, edge2_2));

        // Average the normals from both triangles
        glm::vec3 avgNormal = glm::normalize((normal1 + normal2) * 0.5f);

        // Add to all 4 vertices of the quad
        normals[idx0] += avgNormal;
        normals[idx1] += avgNormal;
        normals[idx2] += avgNormal;
        normals[idx3] += avgNormal;
    }

    // Normalize all accumulated normals
    for (auto& normal : normals) {
        if (glm::length(normal) > 0.0f) {
            normal = glm::normalize(normal);
        }
        else {
            normal = glm::vec3(0.0f, 1.0f, 0.0f); // Default upward normal
        }
    }

    std::cout << "Quad patch normals calculated: " << normals.size() << " normals" << std::endl;
}