#pragma once
#include <vector>
#include "PerlinNoiseGen.h"
#include "HeightMapGen.h"
#include "algorithm"
class FractualNoiseGen : public HeightMapGen
{
private:
	PerlinNoise PerlinGen; // Creates Simplex Noise
	// Defining Modifiers
	float MAX_HEIGHT = 1.0f; // Max elevation for a pixel
	float MIN_HEIGHT = -1.0f; // Min elevationn
	float amplitude = MAX_HEIGHT / 2; // How extreme can the pixels go between max and min
	float frequency = 0.05; // How detailed is the grid ( zooming factor )
	float octaves = 3; // How many iterations
	float lacunarity = 2.0; // Increase Frequency after each iteration
	float persistence = 0.5; // decrease Frequency after each iteration
	float clamp(float d, float min, float max) {
		const float t = d < min ? min : d;
		return t > max ? max : t;
	}
public:
    virtual std::vector<float> GenerateHightMap(int height, int width) override {
        std::vector<float> data(height * width); // Resize the vector to the appropriate size
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                int idx = (i * width + j);
                float elevation = 0.0f;
                float t_frequency = frequency;
                float t_amplitude = amplitude;
                for (int k = 0; k < octaves; k++) {
                    float sample_x = j * t_frequency; // Define our grid points affected by our frequency
                    float sample_y = i * t_frequency;
                    elevation += PerlinGen.perlin(sample_x/height, sample_y/width) * t_amplitude; // Generate the noise for it
                    t_frequency *= lacunarity;
                    t_amplitude *= persistence;
                }

                elevation = clamp(elevation,MIN_HEIGHT,MAX_HEIGHT);

                data[idx] = elevation;
            }
        }
        return data;
    };

};



