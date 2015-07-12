#include "stdafx.h"
#include "PlanetarySystem.h"


PlanetarySystem::PlanetarySystem(Vector3 _startPos) : startPos(_startPos)
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
	star.worldPosition = startPos;

	star.speed = 0;

	nbPlanet = 0;
	float distance = 2000 + rand() % 500;
	while (distance < 12000) {
		planets.push_back(Planet(distance));
		planets[nbPlanet].worldPosition = addVector3(startPos, makeVector3(distance, 0.0f, 0.0f));
		planets[nbPlanet].speed = chooseSpeed(distance);
		planets[nbPlanet].timeOffset = rand() % 360;
		++nbPlanet;
		distance += 750 + rand() % 500;		
	}
}

float PlanetarySystem::chooseSpeed(float distance)
{
	return (100 - (distance - 2000) / 100 + 25) * 0.0001;
}