#pragma once
#include <vector>
class HeightMapGen
{
public:
	virtual std::vector<float> GenerateHightMap(int height, int width) =0;
};





