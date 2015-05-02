#include "stdafx.h"
#include "Scene.h"
#include "CameraSystem.h"
#include "ResourcesManager.h"

Scene::Scene()
{
	children = new std::vector<Entity*>();
	lights = new std::vector<Entity *>();

	ResourcesManager* resources = &ResourcesManager::getResourcesManager();

	Entity* _entity = new Entity((resources->getVertexBufferArray()->at(0)), makeVector3(-0.5f, -0.5f, 0.0f));
	Entity* _entity2 = new Entity((resources->getVertexBufferArray()->at(1)), makeVector3(0.0f, 0.0f, 0.0f));
	//Entity* _entity2 = new Entity((resources->getVertexBufferArray()->at(0)), makeVector3(0.0f, 0.0f, 0.0f));
	//_entity->set_rotation(makeVector3(90.0f, 0.0f, 0.0f));
	_entity->set_rotationVelocity(makeVector3(1.0f, 1.0f, 1.0f));
	//_entity->set_velocity(makeVector3(0.01f, 0.0f, 0.0f));
	_entity2->set_rotationVelocity(makeVector3(-1.0f, 1.0f, -1.0f));
	//_entity2->set_scale(makeVector3(1.1f, 1.1f, 1.1f));

	/*for (int i = 0; i < 100000; ++i)
	{
	Entity* _entity3 = new Entity((resources->getVertexBufferArray()->at(2)), makeVector3(cos(i * 0.05f), -sin(i * 0.05f),i * 0.05f));
	_entity3->set_rotationVelocity(makeVector3(1.0f, 1.0f, 1.0f));
	children->push_back(_entity3);
	}*/

	children->push_back(_entity);
	children->push_back(_entity2);

	Entity* camera = new Entity(NULL, makeVector3(0.0f, 0.0f, 4.0f));
	camera->set_eyeVector(makeVector3(0.0f, 0.0f, 0.0f));
	camera->set_upVector(makeVector3(0.0f, 1.0f, 0.0f));
	//camera->set_velocity(makeVector3(0.0f, 0.01f, 0.01f));

	children->push_back(camera);

	CameraSystem* cameraSystem = &CameraSystem::getCameraSystem();
	cameraSystem->setCurrentCamera(camera);

	Entity* _lightEmitter = new Entity(NULL, makeVector3(-5.0f, -5.0f, 5.0f));
	lights->push_back(_lightEmitter);
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

std::vector<Entity *>* Scene::getLights()
{
	return lights;
}