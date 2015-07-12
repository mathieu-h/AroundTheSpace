#include "stdafx.h"
#include "PlanetarySystem.h"


PlanetarySystem::PlanetarySystem(Vector3 _startPos) : startPos(_startPos)
{
	GeneratePlanetarySystem();
}

PlanetarySystem::PlanetarySystem(int starNumber, int numberplanets)
{
	GenerateGalaxy(starNumber, numberplanets);
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
	int _nbPlanet = rand() % 5 + 5;
	float distance = 2000 + rand() % 100;
	
	
	for (int i = 0; i < _nbPlanet; ++i) {
		++nbPlanet;
		planets.push_back(Planet());
		planets[i].worldPosition = addVector3(startPos, makeVector3(distance, 0.0f, 0.0f));
		planets[i].speed = chooseSpeed();
		planets[i].timeOffset = rand() % 360;
		distance += 1000 + rand() % 100;
		//if (distance > 650)
		//	break;				
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

void PlanetarySystem::GenerateGalaxy(int starNumber, int numberplanets)
{
	for (int i = 0; i < starNumber; i++)
	{
		star = Star();
		star.worldPosition = makeVector3(0.0f,0.0f,0.0f);
		_starSystem.push_back(star);
	}
	
	for (int i = 0; i < numberplanets; ++i) {
		planets.push_back(Planet());
		planets[i].speed = chooseSpeed();
		planets[i].timeOffset = rand() % 360;		
	}
}