//
//  PlayerInputSystem.h
//  SimpleFPS
//
//  Created by Dimitriy Dounaev on 26/11/13.
//  Copyright (c) 2013 Dimitriy Dounaev. All rights reserved.
//

#ifndef __SimpleFPS__PlayerInputSystem__
#define __SimpleFPS__PlayerInputSystem__

#include "stdafx.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include "Entity.h"
#include "Vector2.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PlayerInputSystem
{
private:

	Vector2 _lastMousePosition;

	Vector3 _eyeVector;

	GLFWwindow *_window;

	Entity *_currentPlayer;
	/*
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);*/

	GLfloat yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
	GLfloat pitch = 0.0f;

	GLfloat lastX = 1280.0f / 2, lastY = 720.0f / 2;

	PlayerInputSystem();
	~PlayerInputSystem();

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void do_movement();

	int keys[1024];

	// Deltatime
	GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
	GLfloat lastFrame = 0.0f;  	// Time of last frame

public:

	void setCurrentPlayer(Entity *newPlayer);

	void update();

	static PlayerInputSystem& getPlayerInputSystem();
	static void destroyPlayerInputSystem();

	static void keyCallbackFun(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callbackFun(GLFWwindow* window, double xpos, double ypos);
};

#endif /* defined(__SimpleFPS__PlayerInputSystem__) */