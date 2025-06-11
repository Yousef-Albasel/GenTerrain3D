#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <iostream>

class Heightmap {
private:
    float RandomFloat();


protected:
	const float m_size;
	int m_width;
	int m_depth;

	int vertexCount;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;    
    std::vector<float> texCoords;
    std::vector<glm::vec3> normals;
    void DebugNormals();

public:
    // Constructor
    Heightmap(float size, int width, int depth);

    // Initialize Height Map
    void InitializeHeightMap();

    // Generate Vertices
    bool GenerateVertices();
    
    // Generate Texture Coordinates
    void GenerateTextureCoords();
   
    // Generate normal vectors

    void CalculateNormals();
   
    // Getters and Setters
    float getSize() const { return m_size; }
    float getWidth() const { return m_width; }
    float getDepth() const { return m_depth; }
    float GetHeightAt(int x, int z) const;
    void SetHeightAt(int x, int z, float height);
    std::vector<float> GetVertices() { return vertices; };
    std::vector<float> GetTexCoord() { return texCoords; };
    std::vector<unsigned int> GetIndices() { return indices; };
    std::vector<float> GetNormals() {
            std::vector<float> flatNormals;
            flatNormals.reserve(normals.size() * 3); // Reserve space for x, y, z of each normal
            for (const auto& normal : normals) {
                flatNormals.push_back(normal.x);
                flatNormals.push_back(normal.y);
                flatNormals.push_back(normal.z);
            }
            return flatNormals;
    };
    std::vector<glm::vec3> GetNormalsVec() { return normals; };
    // Normalize Heights
    void NormalizeHeights(float MaxRange, float MinRange);

    // Get Min and Max Heights
    void GetMinMax(float& Min, float& Max);

    float RandomFloatRange(float Start, float End);

    // Apply FIR Filter
    void ApplyFIRFilter(float filter);

    // Apply FIR Filter for a Single Point
    float FIRFilterSinglePoint(int x, int z, float PrevVal, float Filter);

    // Print Vertices
    void PrintVertices();



};