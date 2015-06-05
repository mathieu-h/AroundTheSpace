#include "stdafx.h"
#include "RenderSystem.h"
#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace std;

Entity* RenderSystem::getCurrentCamera()
{
	return _currentCamera;
}

void RenderSystem::setCurrentCamera(Entity* currentCamera)
{
	_currentCamera = currentCamera;
}

RenderSystem::RenderSystem() : _window(glfwGetCurrentContext()), _cameraSystem(&CameraSystem::getCameraSystem())
{
	_currentCamera = _cameraSystem->getCurrentCamera();
}


RenderSystem::~RenderSystem()
{
}

void RenderSystem::render(std::vector<Entity*> *children)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (std::vector<Entity*>::iterator iterator = children->begin(); iterator != children->end(); iterator++)
	{
		Entity* entity = *iterator;
		if (entity->get_vertexBuffer() != NULL) {

			

			glUseProgram(entity->get_vertexBuffer()->get_Shader()->getProgramHandle());

			if (entity->get_vertexBuffer()->get_Shader()->get_aTextCoords() != -1) {

				glm::mat4 m_projectionMatrix = glm::perspective(45.0f, (1280.0f / 720.0f), 0.1f, 1000.0f);
				GLuint transformLoc = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "projectionMatrix");
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));

				glm::mat4 m_modelMatrix;

				m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(entity->get_position().x, entity->get_position().y, entity->get_position().z));

				m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
				m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
				m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().z, glm::vec3(0.0f, 0.0f, 1.0f));

				GLuint transformLoc2 = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "modelMatrix");
				glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));

				GLfloat radius = 1.0f;
				GLfloat camX = sin(glfwGetTime()) * radius;
				GLfloat camZ = cos(glfwGetTime()) * radius;

				glm::mat4 view;
				view = glm::lookAt(glm::vec3(_currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z),
					glm::vec3(_currentCamera->get_eyeVector().x, _currentCamera->get_eyeVector().y, _currentCamera->get_eyeVector().z),
					glm::vec3(_currentCamera->get_upVector().x, _currentCamera->get_upVector().y, _currentCamera->get_upVector().z));

				GLuint transformLoc3 = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "viewMatrix");
				glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(view));

				GLint lightPosLoc = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "lightPos");
				glUniform3f(lightPosLoc, 5.0f, 5.0f, 5.0f);

				GLint viewPosLoc = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "viewPos");
				glUniform3f(viewPosLoc, _currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z);

			}
			else {
				glLoadIdentity();
				gluLookAt(
					_currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z,
					_currentCamera->get_eyeVector().x, _currentCamera->get_eyeVector().y, _currentCamera->get_eyeVector().z,
					_currentCamera->get_upVector().x, _currentCamera->get_upVector().y, _currentCamera->get_upVector().z);

				glTranslatef(entity->get_position().x, entity->get_position().y, entity->get_position().z);

				glRotatef(entity->get_rotation().x, 0.0f, 0.0f, 1.0f);
				glRotatef(entity->get_rotation().y, 0.0f, 1.0f, 1.0f);
				glRotatef(entity->get_rotation().z, 1.0f, 0.0f, 1.0f);

				glScalef(entity->get_scale().x, entity->get_scale().y, entity->get_scale().z);

			}
		
			/*glUniform4f((entity->get_vertexBuffer()->get_Shader())->get_uColor(),
				(entity->get_vertexBuffer()->get_shaderData())->get_uColorValue().x,
				(entity->get_vertexBuffer()->get_shaderData())->get_uColorValue().y,
				(entity->get_vertexBuffer()->get_shaderData())->get_uColorValue().z,
				(entity->get_vertexBuffer()->get_shaderData())->get_uColorValue().w);

			glUniform3f((entity->get_vertexBuffer()->get_Shader())->get_uLightPosition(),
				(entity->get_vertexBuffer()->get_shaderData())->get_uLightPosition().x,
				(entity->get_vertexBuffer()->get_shaderData())->get_uLightPosition().y,
				(entity->get_vertexBuffer()->get_shaderData())->get_uLightPosition().z);*/

			entity->get_vertexBuffer()->configureVertexAttributes();
			entity->get_vertexBuffer()->renderVertexBuffer();
		}

		
	}
	glfwSwapBuffers(_window);
	glfwPollEvents();
}

void RenderSystem::render(std::vector<Entity*> *children, std::vector<light *>* lights)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (std::vector<Entity*>::iterator iterator = children->begin(); iterator != children->end(); iterator++)
	{
		Entity* entity = *iterator;
		
		if (entity->get_vertexBuffer() != NULL) {

			glUseProgram(entity->get_vertexBuffer()->get_Shader()->getProgramHandle());

			

			glm::mat4 m_projectionMatrix = glm::perspective(45.0f, (1280.0f / 720.0f), 0.1f, 1000.0f);
			GLuint transformLoc = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "projectionMatrix");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));

			glm::mat4 m_modelMatrix;

			
			m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(entity->get_position().x, entity->get_position().y, entity->get_position().z));

			m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().z, glm::vec3(0.0f, 0.0f, 1.0f));
			m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
			m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().y, glm::vec3(0.0f, 1.0f, 0.0f));

			glm::vec3 vec(entity->get_scale().x, entity->get_scale().y, entity->get_scale().z);
			m_modelMatrix = glm::scale(m_modelMatrix, vec);
			
			GLuint transformLoc2 = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "modelMatrix");
			glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));

			GLfloat radius = 2.0f;
			GLfloat camX = sin(glfwGetTime()) * radius;
			GLfloat camZ = cos(glfwGetTime()) * radius;

			glm::mat4 view;
			//view = glm::lookAt(glm::vec3(0.0f, 25.0f, -85.0f),
			view = glm::lookAt(glm::vec3(_currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z),
				glm::vec3(_currentCamera->get_eyeVector().x, _currentCamera->get_eyeVector().y, _currentCamera->get_eyeVector().z),
				glm::vec3(_currentCamera->get_upVector().x, _currentCamera->get_upVector().y, _currentCamera->get_upVector().z));

			GLuint transformLoc3 = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "viewMatrix");
			glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(view));
			
			entity->get_vertexBuffer()->renderMaterials();
			renderLights(entity, lights);

			GLint viewPosLoc = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "viewPos");
			glUniform3f(viewPosLoc, _currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z);


			entity->get_vertexBuffer()->configureVertexAttributes();
			entity->get_vertexBuffer()->renderVertexBuffer();
		}
	}
	glfwSwapBuffers(_window);
	glfwPollEvents();
}

RenderSystem& RenderSystem::getRenderSystem()
{
	static RenderSystem * renderSystem = NULL;

	if (renderSystem == NULL)
	{
		renderSystem = new RenderSystem();

		glClearColor(0.0f,0.0f,0.0f,1.0f);

		glMatrixMode(GL_PROJECTION);
		gluPerspective(45.0f, 1600.0f / 900.0f, 0.1, 10000);
		glViewport(0.0f, 0.0f, 1600.0f, 900.0f);
		glMatrixMode(GL_MODELVIEW);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}

	return *renderSystem;
}

void RenderSystem::renderLights(Entity* entity,std::vector<light *>* lights)
{
	glUniform1f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "numberoflights"), lights->size()-1);

	for (int i = 0; i < lights->size(); i++)
	{
		stringstream ss;
		string index;
		ss << i;
		index = ss.str();
		glUniform3f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), ("pointLights["+index+"].position").c_str()), lights->at(i)->get_position().x, lights->at(i)->get_position().y, lights->at(i)->get_position().z);
		glUniform3f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), ("pointLights[" + index + "].ambient").c_str()), lights->at(i)->get_ambient().x, lights->at(i)->get_ambient().y, lights->at(i)->get_ambient().z);
		glUniform3f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), ("pointLights[" + index + "].diffuse").c_str()), lights->at(i)->get_diffuse().x, lights->at(i)->get_diffuse().y, lights->at(i)->get_diffuse().z);
		glUniform3f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), ("pointLights[" + index + "].specular").c_str()), lights->at(i)->get_specular().x, lights->at(i)->get_specular().y, lights->at(i)->get_specular().z);
		glUniform1f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), ("pointLights[" + index + "].constant").c_str()), lights->at(i)->get_constant());
		glUniform1f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), ("pointLights[" + index + "].linear").c_str()), lights->at(i)->get_linear());
		glUniform1f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), ("pointLights[" + index + "].quadratic").c_str()), lights->at(i)->get_quadratic());
	}
}

void RenderSystem::destroyRenderSystem() 
{
		RenderSystem* renderSystem = &getRenderSystem();
	delete renderSystem;
}