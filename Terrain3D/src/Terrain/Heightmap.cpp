#include "Heightmap.h"
// Constructor

Heightmap::Heightmap(float size, int width, int depth)
    : m_size(size), m_width(width), m_depth(depth) {
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
    CalculateNormals();
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
}

void Heightmap::CalculateNormals() {
    // Initialize the normals vector
    normals.resize(vertexCount, glm::vec3(0.0f, 0.0f, 0.0f));

    // Iterate over all triangles and calculate face normals
    for (size_t i = 0; i < indices.size(); i += 3) {
        int idx0 = indices[i];
        int idx1 = indices[i + 1];
        int idx2 = indices[i + 2];

        glm::vec3 v0 = glm::vec3(vertices[idx0 * 3], vertices[idx0 * 3 + 1], vertices[idx0 * 3 + 2]);
        glm::vec3 v1 = glm::vec3(vertices[idx1 * 3], vertices[idx1 * 3 + 1], vertices[idx1 * 3 + 2]);
        glm::vec3 v2 = glm::vec3(vertices[idx2 * 3], vertices[idx2 * 3 + 1], vertices[idx2 * 3 + 2]);

        // Compute two edges of the triangle
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;

        // Compute the face normal (normalized cross product)
        glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

        // Accumulate face normal into each vertex normal
        normals[idx0] += faceNormal;
        normals[idx1] += faceNormal;
        normals[idx2] += faceNormal;
    }

    // Normalize all vertex normals
    for (auto& normal : normals) {
        normal = glm::normalize(normal);
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