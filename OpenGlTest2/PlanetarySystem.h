#pragma once

#include <vector>
#include "Star.h"
#include "Planet.h"


class PlanetarySystem
{
public:
	PlanetarySystem(Vector3 _startPos);
	PlanetarySystem(int starNumber, int numberplanets);
	~PlanetarySystem();

	vector<GameObject> GetGameObject() const;

	int nbPlanet;
	Star star;
	vector<Star> _starSystem;
	vector<Planet> planets;

	Vector3 startPos;
	vector<Vector3> GalaxyStartPos;
private:
	void GeneratePlanetarySystem();
	void GenerateGalaxy(int starNumber, int numberplanets);
	float chooseSpeed();
	
};

