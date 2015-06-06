#include "stdafx.h"
#include "PlayerInputSystem.h"
#include "Matrix3.h"

void PlayerInputSystem::setCurrentPlayer(Entity *newPlayer)
{
	_currentPlayer = newPlayer;
	_eyeVector = normalizeVector3(newPlayer->get_eyeVector());
}

PlayerInputSystem::PlayerInputSystem() : _window(glfwGetCurrentContext())
{
	double x = _lastMousePosition.x;
	double y = _lastMousePosition.y;
	glfwGetCursorPos(_window, &x, &y);
}

PlayerInputSystem::~PlayerInputSystem()
{

}

void PlayerInputSystem::keyCallback(GLFWwindow *window,
	int key,
	int scancode,
	int action,
	int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		printf("tamer");
		if (GLFW_CURSOR_DISABLED == glfwGetInputMode(glfwGetCurrentContext(), GLFW_CURSOR)) {
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

	}

}

void PlayerInputSystem::update()
{

	float moveSpeed = 0.07f;

	if (_currentPlayer == NULL){
		//printf("no current player");
	}
	if (_currentPlayer != NULL &&
		glfwGetInputMode(_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {

		if (glfwGetKey(_window, GLFW_KEY_Z)) {
			//printf("Z pressed");
			_currentPlayer->set_position(addVector3(_currentPlayer->get_position(), makeVector3(0.00f, 0.00f, moveSpeed)));
			_currentPlayer->set_eyeVector(addVector3(_currentPlayer->get_eyeVector(), makeVector3(0.0f, 0.00f, moveSpeed)));
		}

		if (glfwGetKey(_window, GLFW_KEY_S)) {
			//printf("S pressed");
			//_currentPlayer->set_position(subtractVector3(_currentPlayer->get_position(), scalerMultiplyVector3(_eyeVector, 0.07f)));
			_currentPlayer->set_position(subtractVector3(_currentPlayer->get_position(), makeVector3(0.00f, 0.00f, moveSpeed)));
			_currentPlayer->set_eyeVector(subtractVector3(_currentPlayer->get_eyeVector(), makeVector3(0.0f, 0.00f, moveSpeed)));
		}

		if (glfwGetKey(_window, GLFW_KEY_Q)) {
			//printf("Q pressed");
			_currentPlayer->set_position(addVector3(_currentPlayer->get_position(), makeVector3(moveSpeed, 0.0f, 0.0f)));
			_currentPlayer->set_eyeVector(addVector3(_currentPlayer->get_eyeVector(), makeVector3(moveSpeed, 0.0f, 0.0f)));
		}

		if (glfwGetKey(_window, GLFW_KEY_D)) {
			//printf("D pressed");
			//_currentPlayer->set_position(addVector3(_currentPlayer->get_position(), scalerMultiplyVector3(crossProductVector3(_eyeVector, makeVector3(0.0f, 1.0f, 0.0f)), 0.07f)));
			_currentPlayer->set_position(subtractVector3(_currentPlayer->get_position(), makeVector3(moveSpeed, 0.0f, 0.0f)));
			_currentPlayer->set_eyeVector(subtractVector3(_currentPlayer->get_eyeVector(), makeVector3(moveSpeed, 0.0f, 0.0f)));
		}
		//Gestion de la souris dans le déplacement
		double xPos, yPos;
		glfwGetCursorPos(_window, &xPos, &yPos);
		_currentPlayer->set_eyeVector(makeVector3(-xPos, -yPos, 0.0f));
	}
}

void PlayerInputSystem::keyCallbackFun(GLFWwindow *window,
	int key,
	int scancode,
	int action,
	int mods)
{
	PlayerInputSystem *playerInputSystem = &getPlayerInputSystem();
	playerInputSystem->keyCallback(window, key, scancode, action, mods);
}

PlayerInputSystem& PlayerInputSystem::getPlayerInputSystem()
{
	static PlayerInputSystem *playerInputSystem = NULL;

	if (playerInputSystem == NULL) {

		glfwSetKeyCallback(glfwGetCurrentContext(), *keyCallbackFun);
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		playerInputSystem = new PlayerInputSystem();
	}

	return *playerInputSystem;
}

void PlayerInputSystem::destroyPlayerInputSystem()
{
	PlayerInputSystem *playerInputSystem = &getPlayerInputSystem();
	delete playerInputSystem;
}