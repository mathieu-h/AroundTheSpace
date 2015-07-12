#pragma once
#include <iostream>
#include <Windows.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GLU32.lib")
#include<GL/glew.h>
#include<GLFW\glfw3.h>
#include<GL\GLU.h>

#ifndef _SimpleFps_RenderSystem_
#define _SimpleFps_RenderSystem_

#include "ShaderInterface.h"
#include "VertexBuffer.h"
#include <vector>
#include "Entity.h"
#include "light.h"
#include "CameraSystem.h"


class RenderSystem
{
private:
	RenderSystem();
	~RenderSystem();

	GLFWwindow* _window;

	CameraSystem* _cameraSystem;
	Entity* _currentCamera;
	

public:

	Entity* getCurrentCamera();
	void setCurrentCamera(Entity* currentCamera);


	GLuint _cubemaptexture;

	GLuint loadCubemap(vector<const GLchar*> faces);

	void render(std::vector<Entity*> *children);
	void render(std::vector<Entity*> *children, std::vector<light *>* lights);
	void renderOcculus(std::vector<Entity*> *children, std::vector<light *>* lights, GLfloat w, GLfloat h, const glm::mat4 & headPose);
	void renderLights(Entity* entity,std::vector<light *>* lights);
	void setMatrices(Entity* entity, ShaderInterface* shader);
	void RenderSystem::setMatrices(Entity* entity, ShaderInterface* shader, GLfloat w, GLfloat h, const glm::mat4 & headPose);

	static RenderSystem& getRenderSystem();
	static void destroyRenderSystem();

	bool up_down = false;
	float next_Step = 180.0f;

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
};

#endif
