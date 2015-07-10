#pragma once

#include <vector>
#include "Star.h"
#include "Planet.h"


class PlanetarySystem
{
public:
	PlanetarySystem(Vector3 _startPos);
	~PlanetarySystem();

	vector<GameObject> GetGameObject() const;

	int nbPlanet;
	Star star;
	vector<Planet> planets;

	Vector3 startPos;
private:
	void GeneratePlanetarySystem();

	
};

