#include "Heightmap.h"
// Constructor

Heightmap::Heightmap(float size, int width, int depth)
    : m_size(size), m_width(width), m_depth(depth) {
    vertexCount = m_width * m_depth;
    vertices.resize(vertexCount * 3);
    indices.resize((m_width - 1) * (m_depth - 1) * 6);
    srand(static_cast<unsigned int>(time(0)));
}

// Initialize Height Map
void Heightmap::InitializeHeightMap() {
    GenerateVertices();
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