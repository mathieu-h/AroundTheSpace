#include "stdafx.h"
#include "GameManager.h"



GameManager::GameManager(bool running) : 
_running(running), 
_window(glfwGetCurrentContext()),
_render(&RenderSystem::getRenderSystem()), 
_resourcesManager(&ResourcesManager::getResourcesManager()), 
_movementSystem(&MouvementSystem::getMouvementSystem()),
_cameraSystem(&CameraSystem::getCameraSystem()), 
_scene(new Scene()),
_playerInputSystem(&PlayerInputSystem::getPlayerInputSystem())
{
	//_playerInputSystem->setCurrentPlayer(_render->getCurrentCamera());
}


GameManager::~GameManager()
{
	ResourcesManager::destroyResourcesManager();
	RenderSystem::destroyRenderSystem();
	RenderSystem::destroyRenderSystem();
	PlayerInputSystem::destroyPlayerInputSystem();
}


#define Updates_Per_Second 60.0f

void GameManager::runGameLoop()
{
	double lastime = glfwGetTime();
	double deltaTime = 0.0f;


	while (_running) {

		double currentTime = glfwGetTime();
		deltaTime += (currentTime - lastime)*Updates_Per_Second;
		lastime = currentTime;

		while (deltaTime >= 1.0f) {

			_running = !glfwWindowShouldClose(_window);

			_movementSystem->update(_scene->getChildren());
			_playerInputSystem->update();

			--deltaTime;
		}



		_render->render(_scene->getChildren(), _scene->getLights());
	}
}

/*
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS){
		// Do turn camera
		// GameManager::moveCamera();
	}
} */

void GameManager::moveCamera(){
	// TODO
}

GameManager& GameManager::getGameManager()
{
	static GameManager *gamemanager = NULL;

	if (gamemanager == NULL) {

		glfwInit();
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		glfwWindowHint(GLFW_RED_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8);
		glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_ALPHA_BITS, 8);
		glfwWindowHint(GLFW_SAMPLES, 16);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		GLFWwindow* window = glfwCreateWindow(1600, 900, "Fps", NULL, NULL);
		glfwMakeContextCurrent(window);
		//glfwSetKeyCallback(window, key_callback);
		GLenum error = glewInit();

		gamemanager = new GameManager(true);
	}

	return *gamemanager;
}

void GameManager::destroyGameMangaer()
{
	static GameManager *gamemanager = &getGameManager();

	if (gamemanager != NULL) {
		delete gamemanager;
	}
}