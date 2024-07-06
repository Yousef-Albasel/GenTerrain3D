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

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned char* pixelOffset = data + (j + width * i) * nChannels;
            unsigned char y = pixelOffset[0];

            // vertex
            vertices.push_back(-height / 2.0f + height * i / (float)height);   // vx
            vertices.push_back((float)y * yScale - yShift);   // vy
            vertices.push_back(-width / 2.0f + width * j / (float)width);   // vz
        }
    }

    // index generation
    for (unsigned i = 0; i < height - 1; i ++) {
        for (unsigned j = 0; j < width; j ++) {
            for (unsigned k = 0; k < 2; k++) {
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
bool CTerrain::GenerateVertices(HeightMapGen& h_generator, int width, int height, int nChannels)
{
    //std::vector<float> data = h_generator.GenerateHightMap(height, width);
    //if (data.empty()) return false;

    //float yScale = 64.0f / 255.0f, yShift = 16.0f;  // Adjusted scale to match LoadHeightMap

    //for (int i = 0; i < height; i++)
    //{
    //    for (int j = 0; j < width; j++)
    //    {
    //        int idx = (i * width + j);
    //        float y = data[idx];  // Use the height value directly from the generator

    //        // Scale and shift the height value to match LoadHeightMap
    //        y = ((y + 1.0f) / 2.0f) * 255.0f;  // Convert -1.0f to 1.0f range to 0 to 255

    //        // vertex
    //        vertices.push_back(-height / 2.0f + height * i / (float)height);   // vx
    //        vertices.push_back(y * yScale - yShift);   // vy, using scaled y
    //        vertices.push_back(-width / 2.0f + width * j / (float)width);   // vz
    //    }
    //}

    //for (unsigned i = 0; i < height - 1; i++) {
    //    for (unsigned j = 0; j < width; j++) {
    //        for (unsigned k = 0; k < 2; k++) {
    //            indicies.push_back(j + width * (i + k));
    //        }
    //    }
    //}

    //numOfStrips = height - 1;
    //verticesPerStrip = width * 2;
    //printf("\n%d", numOfStrips * verticesPerStrip);
    return true;
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