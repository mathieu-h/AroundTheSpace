#include "stdafx.h"
#include "RenderSystem.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "Constants.h"
#include "OculusManager.h"

#include <glm/gtx/euler_angles.inl>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>

using namespace std;
using namespace glm;

quat RotationBetweenVectors(vec3 start, vec3 dest){
	start = normalize(start);
	dest = normalize(dest);

	float cosTheta = dot(start, dest);
	vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f){
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
		if (length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
		{
			rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);
			if (length2(rotationAxis) < 0.01)
				rotationAxis = cross(vec3(0.0f, 1.0f, 0.0f), start);
		}
			

		rotationAxis = normalize(rotationAxis);
		return angleAxis(360.0f, rotationAxis);
	}

	rotationAxis = cross(start, dest);

	float s = glm::sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
		);

}

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

void RenderSystem::renderOcculus(std::vector<Entity*> *children, std::vector<light *>* lights, GLfloat w, GLfloat h, const glm::mat4 & headPose)
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

			setMatrices(entity, entity->get_vertexBuffer()->get_Shader(), w, h, headPose);
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
	glm::mat4 m_projectionMatrix = glm::perspective(45.0f, (1920.0f / 1080.0f), 0.1f, 10000000000000.0f);
	GLuint transformLoc = glGetUniformLocation(shader->getProgramHandle(), "projectionMatrix");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));

	glm::mat4 m_modelMatrix;
	Vector3 up = _currentCamera->get_upVector();
	Vector3 pos = _currentCamera->get_position();
	Vector3 front = _currentCamera->get_eyeVector();

	glm::mat4 view;

	view = glm::lookAt(glm::vec3(pos.x, pos.y, pos.z),
		glm::vec3(front.x + pos.x, front.y + pos.y, front.z + pos.z),
		glm::vec3(up.x, up.y, up.z));


	if (entity->isarrow)
	{
		Vector3 northPos = makeVector3(0.0f, 0.0f, -1000.0f);
		
		glm::fvec3 v; glm::fvec3 v2;
		v2.x = 0.0f;
		v2.x = 0.0f;
		v2.x = -1.0f;

		v.x = _currentCamera->get_eyeVector().x;
		v.y = _currentCamera->get_eyeVector().y;
		v.z = _currentCamera->get_eyeVector().z;

		quat q = RotationBetweenVectors(v, v2);
		
		//glm::mat4 RotationMatrix = glm::transpose(glm::lookAt(v2, v2, glm::vec3(0.0f, 1.0f, 0.0f)));
		//Now convert to quaternion if you use quaternion and not matrix for the representation of the rotation :
		//quat Rotation = toQuat(RotationMatrix);

		vec3 newUp = q * vec3(0.0f, 1.0f, 0.0f);
		quat rot2 = RotationBetweenVectors(newUp, vec3(0.0f, 1.0f, 0.0f));
		// Because of the 1rst rotation, the up is probably completely screwed up. 
		// Find the rotation between the "up" of the rotated object, and the desired up

		newUp = q * vec3(0.0f, 0.0f, 1.0f);
		quat rot3 = RotationBetweenVectors(newUp, vec3(0.0f, 0.0f, 1.0f));

		newUp = q * vec3(1.0f, 0.0f, 0.0f);
		quat rot4 = RotationBetweenVectors(newUp, vec3(1.0f, 0.0f, 0.0f));

		quat targetOrientation = rot2 * q;
		targetOrientation = rot3 * targetOrientation;
		targetOrientation = rot4 * targetOrientation;

		vec3 rotated_point = eulerAngles(targetOrientation);
		//mat4 RotationMatrix = t(q);

		m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(entity->get_position().x, entity->get_position().y, entity->get_position().z));

		m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(entity->get_vertexBuffer()->middle.x, entity->get_vertexBuffer()->middle.y, entity->get_vertexBuffer()->middle.z));

		m_modelMatrix = glm::rotate(m_modelMatrix, rotated_point.x, glm::vec3(1.0f, 0.0f, 0.0f));
		m_modelMatrix = glm::rotate(m_modelMatrix, rotated_point.y, glm::vec3(0.0f, 1.0f, 0.0f));
		m_modelMatrix = glm::rotate(m_modelMatrix, rotated_point.z, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 vec(entity->get_scale().x, entity->get_scale().y, entity->get_scale().z);
		m_modelMatrix = glm::scale(m_modelMatrix, vec);

		m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(-entity->get_vertexBuffer()->middle.x, -entity->get_vertexBuffer()->middle.y, -entity->get_vertexBuffer()->middle.z));
	}
	else {
		m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(entity->get_position().x, entity->get_position().y, entity->get_position().z));

		m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
		m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
		m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().z, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 vec(entity->get_scale().x, entity->get_scale().y, entity->get_scale().z);
		m_modelMatrix = glm::scale(m_modelMatrix, vec);

		
	}
	

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

	/*GLuint transformNorth = glGetUniformLocation(shader->getProgramHandle(), "northPoint");
	glUniform3f(transformNorth, 0.0f,0.0f,-100000.0f);*/

	

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


void RenderSystem::setMatrices(Entity* entity, ShaderInterface* shader, GLfloat w, GLfloat h, const glm::mat4 & headPose)
{
	glm::mat4 m_projectionMatrix = glm::perspective(45.0f, (w / h), 0.1f, 1000000.0f);
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

	glm::mat4 result;
	glm::mat4 cameraInput;;
	glm::mat4 oculusInput;

	Vector3 pos = _currentCamera->get_position();
	Vector3 front = _currentCamera->get_eyeVector();
	Vector3 up = _currentCamera->get_upVector();

	if (Constants::getConstants()->oculus)
		oculusInput = glm::inverse(headPose);
	
	cameraInput = glm::lookAt(glm::vec3(pos.x, pos.y, pos.z),
		glm::vec3(front.x + pos.x, front.y + pos.y, front.z + pos.z),
		glm::vec3(up.x, up.y, up.z));

	if (Constants::getConstants()->oculus)
		result = oculusInput * cameraInput;
	else
		result = cameraInput;

	if (entity->get_vertexBuffer()->_cube) {

		/*view = glm::mat4(glm::mat3(glm::lookAt(glm::vec3(_currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z),
		glm::vec3(_currentCamera->get_eyeVector().x, _currentCamera->get_eyeVector().y, _currentCamera->get_eyeVector().z),
		glm::vec3(_currentCamera->get_upVector().x, _currentCamera->get_upVector().y, _currentCamera->get_upVector().z))));*/

		glm::mat4 view = glm::mat4(glm::mat3(result));

		GLuint transformLoc3 = glGetUniformLocation(shader->getProgramHandle(), "viewMatrix");
		glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(view));
		glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemaptexture);
		glDepthFunc(GL_LEQUAL);
	}
	else 
	{
		glDepthFunc(GL_LESS);

		GLuint transformLoc3 = glGetUniformLocation(shader->getProgramHandle(), "viewMatrix");
		glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(result));
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

		glClearColor(1.0f,1.0f,1.0f,0.1f);

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

