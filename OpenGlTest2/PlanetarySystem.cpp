#include "stdafx.h"
#include "PlanetarySystem.h"


PlanetarySystem::PlanetarySystem()
{
	GeneratePlanetarySystem();
}


PlanetarySystem::~PlanetarySystem()
{
}


vector<GameObject> PlanetarySystem::GetGameObject() const
{
	vector<GameObject> gameObjects;
	gameObjects.push_back(star);
	for (int i = 0; i < nbPlanet; ++i) {
		gameObjects.push_back(planets[i]);
	}
	return gameObjects;
}


void PlanetarySystem::GeneratePlanetarySystem()
{
	star = Star();
	star.worldPosition = makeVector3(0.0f, 0.0f, 0.0f);

	nbPlanet = rand() % 8 + 5;
	for (int i = 0; i < nbPlanet; ++i) {
		float distance = 50.0f + i * 20.0f;
		planets.push_back(Planet());
		planets[i].worldPosition = makeVector3(distance, 0.0f, 0.0f);
	}
}