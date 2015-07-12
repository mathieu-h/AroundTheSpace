#include "stdafx.h"
#include "RenderSystem.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "Constants.h"
#include "OculusManager.h"

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

	vector<const GLchar*> faces;
	
	faces.push_back("Images/Green Nebula Left TEX.png");
	faces.push_back("Images/Green Nebula Right TEX.png");
	faces.push_back("Images/Green Nebula Top TEX.png");
	faces.push_back("Images/Green Nebula Bottom TEX.png");
	faces.push_back("Images/Green Nebula Front TEX.png");
	faces.push_back("Images/Green Nebula Back TEX.png");
	
	
	/*faces.push_back("Green Nebula Back TEX.png");
	faces.push_back("Green Nebula Bottom TEX.png");
	faces.push_back("Green Nebula Front TEX.png");
	faces.push_back("Green Nebula Left TEX.png");
	faces.push_back("Green Nebula Right TEX.png");
	faces.push_back("Green Nebula Top TEX.png");*/
	/*faces.push_back("right.jpg");
	faces.push_back("left.jpg");
	faces.push_back("top.jpg");
	faces.push_back("bottom.jpg");
	faces.push_back("back.jpg");
	faces.push_back("front.jpg");*/

	_cubemaptexture = loadCubemap(faces);

}


RenderSystem::~RenderSystem()
{
}

void RenderSystem::render(std::vector<Entity*> *children, std::vector<light *>* lights)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (std::vector<Entity*>::iterator iterator = children->begin(); iterator != children->end(); iterator++)
	{
		Entity* entity = *iterator;
		
		if (entity->get_vertexBuffer() != NULL) {

			glUseProgram(entity->get_vertexBuffer()->get_Shader()->getProgramHandle());

			

			/*glm::mat4 m_projectionMatrix = glm::perspective(45.0f, (1280.0f / 720.0f), 0.1f, 1000.0f);
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
			GLfloat camX = sin(glfwGetTime()) * radiusr
			GLfloat camZ = cos(glfwGetTime()) * radius;

			glm::mat4 view;

			Vector3 pos = _currentCamera->get_position();
			Vector3 front = _currentCamera->get_eyeVector();
			Vector3 up = _currentCamera->get_upVector();

			view = glm::lookAt(glm::vec3(pos.x, pos.y, pos.z),
				glm::vec3(front.x + pos.x, front.y + pos.y, front.z + pos.z),
				glm::vec3(up.x, up.y, up.z));

			GLuint transformLoc3 = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "viewMatrix");
			glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(view));
			
			entity->get_vertexBuffer()->renderMaterials();
			

			GLint viewPosLoc = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "viewPos");
			glUniform3f(viewPosLoc, _currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z);*/

			setMatrices(entity, entity->get_vertexBuffer()->get_Shader());
			renderLights(entity, lights);
			

			entity->get_vertexBuffer()->configureVertexAttributes();
			entity->get_vertexBuffer()->renderVertexBuffer();
		}
	}
	if (!Constants::getConstants()->oculus)
	{
		glfwSwapBuffers(_window);
	}
	glfwPollEvents();
}

void RenderSystem::renderOcculus(std::vector<Entity*> *children, std::vector<light *>* lights, GLfloat w, GLfloat h)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (std::vector<Entity*>::iterator iterator = children->begin(); iterator != children->end(); iterator++)
	{
		Entity* entity = *iterator;

		if (entity->get_vertexBuffer() != NULL) {

			glUseProgram(entity->get_vertexBuffer()->get_Shader()->getProgramHandle());



			/*glm::mat4 m_projectionMatrix = glm::perspective(45.0f, (1280.0f / 720.0f), 0.1f, 1000.0f);
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
			GLfloat camX = sin(glfwGetTime()) * radiusr
			GLfloat camZ = cos(glfwGetTime()) * radius;

			glm::mat4 view;

			Vector3 pos = _currentCamera->get_position();
			Vector3 front = _currentCamera->get_eyeVector();
			Vector3 up = _currentCamera->get_upVector();

			view = glm::lookAt(glm::vec3(pos.x, pos.y, pos.z),
			glm::vec3(front.x + pos.x, front.y + pos.y, front.z + pos.z),
			glm::vec3(up.x, up.y, up.z));

			GLuint transformLoc3 = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "viewMatrix");
			glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(view));

			entity->get_vertexBuffer()->renderMaterials();


			GLint viewPosLoc = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "viewPos");
			glUniform3f(viewPosLoc, _currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z);*/

			setMatrices(entity, entity->get_vertexBuffer()->get_Shader(), w, h);
			renderLights(entity, lights);


			entity->get_vertexBuffer()->configureVertexAttributes();
			entity->get_vertexBuffer()->renderVertexBuffer();
		}
	}
	if (!Constants::getConstants()->oculus)
	{
		glfwSwapBuffers(_window);
	}
	glfwPollEvents();
}

void RenderSystem::setMatrices(Entity* entity, ShaderInterface* shader)
{
	glm::mat4 m_projectionMatrix = glm::perspective(45.0f, (1920.0f / 1080.0f), 0.1f, 10000.0f);
	GLuint transformLoc = glGetUniformLocation(shader->getProgramHandle(), "projectionMatrix");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));

	glm::mat4 m_modelMatrix;

	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(entity->get_position().x, entity->get_position().y, entity->get_position().z));

	m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().z, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec3 vec(entity->get_scale().x, entity->get_scale().y, entity->get_scale().z);
	m_modelMatrix = glm::scale(m_modelMatrix, vec);

	float f = glfwGetTime();

	if (up_down == true) {
		f = next_Step + (next_Step - f);
	}

	if (f >= next_Step)
	{
		next_Step += 180;
		up_down = !up_down;
	}
	GLuint transformLo = glGetUniformLocation(shader->getProgramHandle(), "time");
	glUniform1f(transformLo, f);

	GLuint transformLoc2 = glGetUniformLocation(shader->getProgramHandle(), "modelMatrix");
	glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));

	glm::mat4 view;

	Vector3 pos = _currentCamera->get_position();
	Vector3 front = _currentCamera->get_eyeVector();
	Vector3 up = _currentCamera->get_upVector();

	view = glm::lookAt(glm::vec3(pos.x, pos.y, pos.z),
		glm::vec3(front.x + pos.x, front.y + pos.y, front.z + pos.z),
		glm::vec3(up.x, up.y, up.z));
	if (entity->get_vertexBuffer()->_cube) {

		/*view = glm::mat4(glm::mat3(glm::lookAt(glm::vec3(_currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z),
			glm::vec3(_currentCamera->get_eyeVector().x, _currentCamera->get_eyeVector().y, _currentCamera->get_eyeVector().z),
			glm::vec3(_currentCamera->get_upVector().x, _currentCamera->get_upVector().y, _currentCamera->get_upVector().z))));*/

		view = glm::mat4(glm::mat3(glm::lookAt(glm::vec3(pos.x, pos.y, pos.z),
			glm::vec3(front.x + pos.x, front.y + pos.y, front.z + pos.z),
			glm::vec3(up.x, up.y, up.z))));

		GLuint transformLoc3 = glGetUniformLocation(shader->getProgramHandle(), "viewMatrix");
		glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(view));
		glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemaptexture);
		glDepthFunc(GL_LEQUAL);
	}
	else {
		glDepthFunc(GL_LESS);

		GLuint transformLoc3 = glGetUniformLocation(shader->getProgramHandle(), "viewMatrix");
		glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3f(glGetUniformLocation(shader->getProgramHandle(), "cameraPos"), _currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z);
	

		glUniform3f(glGetUniformLocation(shader->getProgramHandle(), "originPos"), entity->get_vertexBuffer()->originPos.x, entity->get_vertexBuffer()->originPos.y, entity->get_vertexBuffer()->originPos.z);
		glUniform3f(glGetUniformLocation(shader->getProgramHandle(), "startPos"), entity->get_position().x, entity->get_position().y, entity->get_position().z);
		glUniform1f(glGetUniformLocation(shader->getProgramHandle(), "planetSpeed"), entity->get_vertexBuffer()->speed);
		glUniform1f(glGetUniformLocation(shader->getProgramHandle(), "timeOffset"), entity->get_vertexBuffer()->timeOffset);

		if (entity->get_vertexBuffer()->isStart)
		{
			glUniform3f(glGetUniformLocation(shader->getProgramHandle(), "starPos"), entity->get_position().x, entity->get_position().y, entity->get_position().z);

		}


		entity->get_vertexBuffer()->renderMaterials();

		GLint viewPosLoc = glGetUniformLocation(shader->getProgramHandle(), "viewPos");
		glUniform3f(viewPosLoc, _currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z);
	}
	
}


void RenderSystem::setMatrices(Entity* entity, ShaderInterface* shader, GLfloat w, GLfloat h)
{
	glm::mat4 m_projectionMatrix = glm::perspective(45.0f, (w / h), 0.1f, 10000.0f);
	GLuint transformLoc = glGetUniformLocation(shader->getProgramHandle(), "projectionMatrix");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));

	glm::mat4 m_modelMatrix;

	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(entity->get_position().x, entity->get_position().y, entity->get_position().z));

	m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().z, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec3 vec(entity->get_scale().x, entity->get_scale().y, entity->get_scale().z);
	m_modelMatrix = glm::scale(m_modelMatrix, vec);

	float f = glfwGetTime();

	if (up_down == true) {
		f = next_Step + (next_Step - f);
	}

	if (f >= next_Step)
	{
		next_Step += 180;
		up_down = !up_down;
	}
	GLuint transformLo = glGetUniformLocation(shader->getProgramHandle(), "time");
	glUniform1f(transformLo, f);

	GLuint transformLoc2 = glGetUniformLocation(shader->getProgramHandle(), "modelMatrix");
	glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));

	glm::mat4 view;

	Vector3 pos = _currentCamera->get_position();
	Vector3 front = _currentCamera->get_eyeVector();
	Vector3 up = _currentCamera->get_upVector();

	view = glm::lookAt(glm::vec3(pos.x, pos.y, pos.z),
		glm::vec3(front.x + pos.x, front.y + pos.y, front.z + pos.z),
		glm::vec3(up.x, up.y, up.z));
	if (entity->get_vertexBuffer()->_cube) {

		/*view = glm::mat4(glm::mat3(glm::lookAt(glm::vec3(_currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z),
		glm::vec3(_currentCamera->get_eyeVector().x, _currentCamera->get_eyeVector().y, _currentCamera->get_eyeVector().z),
		glm::vec3(_currentCamera->get_upVector().x, _currentCamera->get_upVector().y, _currentCamera->get_upVector().z))));*/

		view = glm::mat4(glm::mat3(glm::lookAt(glm::vec3(pos.x, pos.y, pos.z),
			glm::vec3(front.x + pos.x, front.y + pos.y, front.z + pos.z),
			glm::vec3(up.x, up.y, up.z))));

		GLuint transformLoc3 = glGetUniformLocation(shader->getProgramHandle(), "viewMatrix");
		glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(view));
		glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemaptexture);
		glDepthFunc(GL_LEQUAL);
	}
	else {
		glDepthFunc(GL_LESS);

		GLuint transformLoc3 = glGetUniformLocation(shader->getProgramHandle(), "viewMatrix");
		glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3f(glGetUniformLocation(shader->getProgramHandle(), "cameraPos"), _currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z);


		glUniform3f(glGetUniformLocation(shader->getProgramHandle(), "originPos"), entity->get_vertexBuffer()->originPos.x, entity->get_vertexBuffer()->originPos.y, entity->get_vertexBuffer()->originPos.z);
		glUniform3f(glGetUniformLocation(shader->getProgramHandle(), "startPos"), entity->get_position().x, entity->get_position().y, entity->get_position().z);
		glUniform1f(glGetUniformLocation(shader->getProgramHandle(), "planetSpeed"), entity->get_vertexBuffer()->speed);
		glUniform1f(glGetUniformLocation(shader->getProgramHandle(), "timeOffset"), entity->get_vertexBuffer()->timeOffset);

		if (entity->get_vertexBuffer()->isStart)
		{
			glUniform3f(glGetUniformLocation(shader->getProgramHandle(), "starPos"), entity->get_position().x, entity->get_position().y, entity->get_position().z);

		}


		entity->get_vertexBuffer()->renderMaterials();

		GLint viewPosLoc = glGetUniformLocation(shader->getProgramHandle(), "viewPos");
		glUniform3f(viewPosLoc, _currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z);
	}

}

GLuint RenderSystem::loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

RenderSystem& RenderSystem::getRenderSystem()
{
	static RenderSystem * renderSystem = NULL;

	if (renderSystem == NULL)
	{
		renderSystem = new RenderSystem();

		glClearColor(0.0f,0.0f,0.0f,0.1f);

		glMatrixMode(GL_PROJECTION);
		gluPerspective(45.0f, 1600.0f / 900.0f, 0.1, 10000);
		glViewport(0.0f, 0.0f, 1600.0f, 900.0f);
		glMatrixMode(GL_MODELVIEW);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	return *renderSystem;
}

void RenderSystem::renderLights(Entity* entity,std::vector<light *>* lights)
{
	glUniform1f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "numberoflights"), lights->size());

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
