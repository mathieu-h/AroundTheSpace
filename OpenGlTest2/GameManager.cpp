#include "stdafx.h"
#include "GameManager.h"
#include "OculusManager.h"
#include "Constants.h"



GameManager::GameManager(bool running) : 
_running(running), 
_window(glfwGetCurrentContext()),
_render(&RenderSystem::getRenderSystem()), 
_resourcesManager(&ResourcesManager::getResourcesManager()), 
_cameraSystem(&CameraSystem::getCameraSystem()), 
_scene(new Scene()), 
_playerInputSystem(&PlayerInputSystem::getPlayerInputSystem())
{
}


GameManager::~GameManager()
{
	ResourcesManager::destroyResourcesManager();
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
			_playerInputSystem->update();

			--deltaTime;
		}



		_render->render(_scene->getChildren(), _scene->getLights());
	}
}


GameManager& GameManager::getGameManager()
{
	static GameManager *gamemanager = NULL;

	if (gamemanager == NULL) {
		if (Constants::oculus)
		{
			OculusManager oculus = OculusManager::getOculusManager();
		}
		else {
			glfwInit();
			glfwWindowHint(GLFW_DEPTH_BITS, 24);
			glfwWindowHint(GLFW_RED_BITS, 8);
			glfwWindowHint(GLFW_BLUE_BITS, 8);
			glfwWindowHint(GLFW_GREEN_BITS, 8);
			glfwWindowHint(GLFW_ALPHA_BITS, 8);
			glfwWindowHint(GLFW_SAMPLES, 16);
			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
			GLFWwindow* window = glfwCreateWindow(Constants::window_width, Constants::window_heigh, Constants::game_name(), NULL, NULL);
			glfwMakeContextCurrent(window);
			//glfwSetKeyCallback(window, key_callback);
			GLenum error = glewInit();
		}

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