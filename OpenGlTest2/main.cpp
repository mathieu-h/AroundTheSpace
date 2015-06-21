// OpenGlTest2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cstdio>
#include <time.h> 

#include "GameManager.h"

#include <stdlib.h>

#include <vector>
#include <list>

int main(int argc, char** argv)
{
	srand(time(NULL));
	GameManager* game = &GameManager::getGameManager();
	game->runGameLoop();

	return 0;
}

