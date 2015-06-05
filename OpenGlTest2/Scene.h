#pragma once
#include "stdafx.h"

#ifndef _SimpleFps_Scene
#define _SimpleFps_Scene

#include "Entity.h"
#include "light.h"
#include <vector>

class Scene
{
private:

	std::vector<Entity *> *children;
	std::vector<light *> *_lights;
public:

	std::vector<Entity *> * getChildren();
	std::vector<light *> *getLights();
	Scene();
	~Scene();
};

#endif