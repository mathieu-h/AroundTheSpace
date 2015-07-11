#include "stdafx.h"
#include "PlayerInputSystem.h"
#include "Matrix3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void PlayerInputSystem::setCurrentPlayer(Entity *newPlayer)
{
	_currentPlayer = newPlayer;
	_eyeVector = normalizeVector3(newPlayer->get_eyeVector());
}

PlayerInputSystem::PlayerInputSystem() : _window(glfwGetCurrentContext())
{
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
		if (GLFW_CURSOR_DISABLED == glfwGetInputMode(glfwGetCurrentContext(), GLFW_CURSOR)) {
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void PlayerInputSystem::do_movement(){
	//GLfloat cameraSpeed = 5.0f * deltaTime;

	glm::vec3 cameraPos = glm::vec3(_currentPlayer->get_position().x, _currentPlayer->get_position().y, _currentPlayer->get_position().z);
	glm::vec3 cameraFront = glm::vec3(_currentPlayer->get_eyeVector().x, _currentPlayer->get_eyeVector().y, _currentPlayer->get_eyeVector().z);
	glm::vec3 cameraUp = glm::vec3(_currentPlayer->get_upVector().x, _currentPlayer->get_upVector().y, _currentPlayer->get_upVector().z);

	GLfloat cameraSpeed = 50.0f * deltaTime;
	if (keys[GLFW_KEY_Z])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_Q])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	_currentPlayer->set_position(makeVector3(cameraPos.x, cameraPos.y, cameraPos.z));
	//_currentPlayer->set_eyeVector(makeVector3(cameraFront.x, cameraFront.y, cameraFront.z));
	//_currentPlayer->set_upVector(makeVector3(cameraPos.x, cameraPos.y, cameraPos.z));
}

void PlayerInputSystem::update()
{

	//printf("Update \n");
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	do_movement();

	/*
	float moveSpeed = 0.27f;
	double xPos, yPos;
	glfwGetCursorPos(_window, &xPos, &yPos);
	Vector3 currentEyeVector = _currentPlayer->get_eyeVector();
	Vector3 currentPosition = _currentPlayer->get_position();

	if (_currentPlayer != NULL && glfwGetInputMode(_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {

		if (glfwGetKey(_window, GLFW_KEY_Z)) {
			_currentPlayer->set_position(addVector3(_currentPlayer->get_position(), scalerMultiplyVector3(_eyeVector, moveSpeed)));
		}

		if (glfwGetKey(_window, GLFW_KEY_S)) {
			_currentPlayer->set_position(subtractVector3(_currentPlayer->get_position(), scalerMultiplyVector3(_eyeVector, moveSpeed)));
		}

		if (glfwGetKey(_window, GLFW_KEY_Q)) {
			_currentPlayer->set_position(subtractVector3(_currentPlayer->get_position(), scalerMultiplyVector3(crossProductVector3(_eyeVector, makeVector3(0.0f, 1.0f, 0.0f)), moveSpeed)));
		}

		if (glfwGetKey(_window, GLFW_KEY_D)) {
			_currentPlayer->set_position(addVector3(_currentPlayer->get_position(), scalerMultiplyVector3(crossProductVector3(_eyeVector, makeVector3(0.0f, 1.0f, 0.0f)), moveSpeed)));
		}
		*/
			/*
		//Gestion de la souris dans le déplacement
		_currentPlayer->set_eyeVector(makeVector3(-xPos, -yPos, 0.0f));*/
		
		// Mouse Valentin
		/*
		Vector2 currentMousePosition;
		double x;
		double y;
		glfwGetCursorPos(_window, &x, &y);
		GLfloat x3 = x;
		GLfloat y3 = y;
		currentMousePosition.x = x3;
		currentMousePosition.y = y3;

		_eyeVector = transformVector3(_eyeVector, makeRotationMatrix3((3.14f / (4.0f*45.0f))*(-(currentMousePosition.x - _lastMousePosition.x)), 0.0f, 1.0f, 0.0f));
		_eyeVector = transformVector3(_eyeVector, makeRotationMatrix3((3.14f / (4.0f*45.0f))*((currentMousePosition.y - _lastMousePosition.y)), 1.0f, 0.0f, 0.0f));
		
		double x2; //= _lastMousePosition.x;
		double y2; //= _lastMousePosition.y;
		glfwGetCursorPos(_window, &x2, &y2);
		_lastMousePosition.x = x2;
		_lastMousePosition.y = y2;
		_currentPlayer->set_eyeVector(addVector3(_currentPlayer->get_position(), _eyeVector));
		*/

	//	_currentPlayer->set_eyeVector(addVector3(_currentPlayer->get_position(), _eyeVector));
	//}
}

void PlayerInputSystem::mouse_callback(GLFWwindow* window, double xpos, double ypos){
	
	//glm::vec3 cameraFront = glm::vec3(_currentPlayer->get_eyeVector().x, _currentPlayer->get_eyeVector().y, _currentPlayer->get_eyeVector().z);

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.2f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	_currentPlayer->set_eyeVector(makeVector3(front.x, front.y, front.z));

	/*
	Vector2 currentMousePosition;
	double x;
	double y;
	glfwGetCursorPos(_window, &x, &y);
	GLfloat x3 = x;
	GLfloat y3 = y;
	currentMousePosition.x = x3;
	currentMousePosition.y = y3;

	_eyeVector = transformVector3(_eyeVector, makeRotationMatrix3((3.14f / (4.0f*45.0f))*(-(currentMousePosition.x - _lastMousePosition.x)), 0.0f, 1.0f, 0.0f));
	_eyeVector = transformVector3(_eyeVector, makeRotationMatrix3((3.14f / (4.0f*45.0f))*((currentMousePosition.y - _lastMousePosition.y)), 1.0f, 0.0f, 0.0f));

	double x2; //= _lastMousePosition.x;
	double y2; //= _lastMousePosition.y;
	glfwGetCursorPos(_window, &x2, &y2);
	_lastMousePosition.x = x2;
	_lastMousePosition.y = y2;
	_currentPlayer->set_eyeVector(addVector3(_currentPlayer->get_position(), _eyeVector));
	*/
}

void PlayerInputSystem::mouse_callbackFun(GLFWwindow* window, double xpos, double ypos){
	PlayerInputSystem *playerInputSystem = &getPlayerInputSystem();
	playerInputSystem->mouse_callback(window, xpos, ypos);
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
		glfwSetCursorPosCallback(glfwGetCurrentContext(), *mouse_callbackFun);
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