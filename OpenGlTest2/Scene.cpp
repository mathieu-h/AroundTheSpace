#include "stdafx.h"
#include "Scene.h"
#include "CameraSystem.h"
#include "ResourcesManager.h"
#include "materials.h"
#include "GameObject.h"
#include "Star.h"

Scene::Scene()
{
	children = new std::vector<Entity*>();
	
	ResourcesManager* resources = &ResourcesManager::getResourcesManager();

	for (int i = 0; i < resources->getVertexBufferArray()->size() - 1; i++)
	{
		Entity* _entity;
		{
			_entity = new Entity((resources->getVertexBufferArray()->at(i)), resources->_planets.at(i).worldPosition);
		}
		_entity->set_scale(makeVector3(1.0f, 1.0f, 1.0f));
		children->push_back(_entity);
	}

	//Entity* _entity = new Entity((resources->getVertexBufferArray()->at(1)), makeVector3(0.0f, 0.0f, 0.0f));
	Entity* _entity2 = new Entity((resources->getVertexBufferArray()->at(resources->getVertexBufferArray()->size() - 1)), makeVector3(0.0f, 0.0f, 0.0f));
	
	//_entity->set_rotationVelocity(makeVector3(0.0f, 0.5f, 0.0f));
	_entity2->set_scale(makeVector3(1.0f, 1.0f, 1.0f));
	//_entity2->set_rotationVelocity(makeVector3(0.0f, 0.1f, 0.0f));
	//_entity2->set_rotation(makeVector3(0.0f, 0.0f, 15.0f));

	//children->push_back(_entity);
	children->push_back(_entity2);

	Entity* camera = new Entity(NULL, makeVector3(0.0f, 1500.0f, -85.0f));
	camera->set_eyeVector(makeVector3(0.0f, 0.0f, -1.0f));
	camera->set_upVector(makeVector3(0.0f, 1.0f, 0.0f));

	children->push_back(camera);

	CameraSystem* cameraSystem = &CameraSystem::getCameraSystem();
	cameraSystem->setCurrentCamera(camera);

	_lights = new std::vector<light*>();

	for (int i = 0; i < resources->_star.size(); i++)
	{
		_lights->push_back(new light(NULL, resources->_star.at(i).worldPosition, makeVector3(0.01f, 0.01f, 0.01f), makeVector3(0.8f, 0.8f, 0.8f), makeVector3(0.8f, 0.8f, 0.8f), 1.0f, 0.045f, 0.0075f));
	}
	
	

	
}


Scene::~Scene()
{
	for (std::vector<Entity*>::iterator iterator = children->begin(); iterator != children->end(); iterator++)
	{
		delete *iterator;
	}
}

std::vector<Entity *>* Scene::getChildren()
{
	return children;
}

std::vector<light *>* Scene::getLights()
{
	return _lights;
}

