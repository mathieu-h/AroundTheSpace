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

	star.speed = chooseSpeed();

	nbPlanet = 0;
	int _nbPlanet = rand() % 5 + 3;
	float distance = 100 + rand() % 100;
	
	
	for (int i = 0; i < _nbPlanet; ++i) {
		++nbPlanet;
		planets.push_back(Planet());
		planets[i].worldPosition = addVector3(startPos, makeVector3(distance, 0.0f, 0.0f));
		planets[i].speed = chooseSpeed();
		planets[i].timeOffset = rand() % 360;
		distance += 25 + rand() % 75;
		if (distance > 650)
			break;

	
		
	}
}

float PlanetarySystem::chooseSpeed()
{
	int choice = rand() % 6;
	float speed;
	switch (choice)
	{
		case 0 :
			speed = 0.001;
			break;
		case 1:
			speed = 0.005;
			break;
		case 2:
			speed = 0.025;
			break;
		case 3:
			speed = 0.01;
			break;
		case 4:
			speed = 0.05;
			break;
		case 5:
			speed = 0.0075;
			break;
	}
	return speed;
}