#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>

class Heightmap {
protected:
	const float m_size;
	int m_width;
	int m_depth;

	int vertexCount;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

public:
    // Constructor
    Heightmap(float size, int width, int depth);

    // Initialize Height Map
    void InitializeHeightMap();

    // Generate Vertices
    bool GenerateVertices();

    // Getters and Setters
    float getSize() const { return m_size; }
    float GetHeightAt(int x, int z) const;
    void SetHeightAt(int x, int z, float height);
    std::vector<float> GetVertices() { return vertices; };
    std::vector<unsigned int> GetIndices() { return indices; };

    // Normalize Heights
    void NormalizeHeights(float MaxRange, float MinRange);

    // Get Min and Max Heights
    void GetMinMax(float& Min, float& Max);

    // Apply FIR Filter
    void ApplyFIRFilter(float filter);

    // Apply FIR Filter for a Single Point
    float FIRFilterSinglePoint(int x, int z, float PrevVal, float Filter);

    // Print Vertices
    void PrintVertices();

};