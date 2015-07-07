#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <noise/noise.h>
#include "noiseutils.h"
#include "GameObject.h"


class Star : public GameObject
{
public:
	Star();
	~Star();

private:
	void generateStar();
	utils::NoiseMap generateHeightMap();
	void generateTexture(utils::NoiseMap heightMap);

	float radius;
	utils::Color compositionColor1;
	utils::Color compositionColor2;
};

