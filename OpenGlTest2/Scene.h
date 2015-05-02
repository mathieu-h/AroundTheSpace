#pragma once
#include "stdafx.h"

#ifndef _SimpleFps_Scene
#define _SimpleFps_Scene

#include "Entity.h"
#include <vector>

class Scene
{
private:

	std::vector<Entity *> *children;
	std::vector<Entity *> *lights;
public:

	std::vector<Entity *> * getChildren();
	std::vector<Entity *> *getLights();
	Scene();
	~Scene();
};

#endif