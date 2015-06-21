#pragma once

#define _USE_MATH_DEFINES
 
#include <math.h>
#include <noise/noise.h>
#include "noiseutils.h"
#include "GameObject.h"


class Planet: public GameObject
{
public:
	Planet();
	~Planet();

private:
	void generatePlanet();
	utils::NoiseMap generateHeightMap();
	void generateTexture(utils::NoiseMap heightMap);

	float radius;
	int temperature;
	utils::Color compositionColor1;
	utils::Color compositionColor2;
	bool water;
	bool liquidWater;
	bool solidWater;
	bool life;
};

