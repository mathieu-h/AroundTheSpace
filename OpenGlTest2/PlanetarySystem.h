#pragma once

#include <time.h> 
#include <vector>
#include "Star.h"
#include "Planet.h"


class PlanetarySystem
{
public:
	PlanetarySystem();
	~PlanetarySystem();

	vector<GameObject> GetGameObject() const;

private:
	void GeneratePlanetarySystem();

	int nbPlanet;
	Star star;
	vector<Planet> planets;
};

