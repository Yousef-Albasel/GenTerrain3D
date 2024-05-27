#include "CTerrain.h"
#include "stdio.h"      
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"
bool CTerrain::LoadHeightMap(const char* mapPath)
{
    int width, height, nChannels;
    unsigned char* data = stbi_load(mapPath,
        &width, &height, &nChannels,
        0);
    if (data == nullptr) {
        return false;
    }
    int total = width * height * nChannels;
    std::printf("%d",total);
    float yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data
    vertices.reserve(width * height * 3);
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            // since the data is 1D, we need to apply this equation to get the right index
            unsigned char* texel = data + (j + width * i) * nChannels;
            unsigned char y = texel[0]; // raw height

            vertices.push_back(-height / 2.0f + height * i / (float)height);   // vx
            vertices.push_back((int)y * yScale - yShift);   // vy
            vertices.push_back(-width / 2.0f + width * j / (float)width);   // vz
        }
    }

    // index generation
    indicies.reserve((height - 1) * width * 2);
    for (unsigned int i = 0; i < height - 1; i++)       // for each row a.k.a. each strip
    {
        for (unsigned int j = 0; j < width; j++)      // for each column
        {
            for (unsigned int k = 0; k < 2; k++)      // for each side of the strip
            {
                indicies.push_back(j + width * (i + k));
            }
        }
    }

    numOfStrips = height - 1;
    verticesPerStrip= width * 2;
    printf("\n%d", numOfStrips * verticesPerStrip);
    stbi_image_free(data);
    return true;    
}

bool CTerrain::GenerateVertices()
{
    return false;
}

//
//
//bool CTerrain::UnloadHeightMap(void)
//{
//    //check to see if the data has been set
//    if (m_heightData.m_pucData)
//    {
//        //delete the data
//        delete[] m_heightData.m_pucData;
//            //reset the map dimensions, also
//            m_heightData.m_iSize = 0;
//    }
//    //the heightmap has been unloaded
//    printf("Successfully unloaded the heightmap\n");
//    return true;
//}