#include "stdafx.h"
#include "VertexBuffer.h"
#include "indices.h"
#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace std;

VertexBuffer::VertexBuffer(const GLvoid *data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset, boolean indicesWith) : _mode(mode), _count(count), _stride(stride), _shader(shader), _shaderData(shaderData), _positionOffset(positionOffset), _normalOffset(normalOffset), _textureCoordOffset(textureCoordOffset)
{
	
	_vexterArrayID = -1;
	_amount = 1;
	if (indicesWith == true)
	{
		glGenVertexArrays(1, &_vexterArrayID);
		glGenBuffers(1, &_vertexBufferID);
		glGenBuffers(1, &_elementBufferObjectID);

		glBindVertexArray(_vexterArrayID);

		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObjectID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		if (_shader->get_aPositionVertex() != -1) {
			glEnableVertexAttribArray(_shader->get_aPositionVertex());
			glVertexAttribPointer(_shader->get_aPositionVertex(), 3, GL_FLOAT, GL_FALSE, _stride, _positionOffset);
		}

		if (_shader->get_aPositionNormals() != -1) {
			glEnableVertexAttribArray(_shader->get_aPositionNormals());
			glVertexAttribPointer(_shader->get_aPositionNormals(), 3, GL_FLOAT, GL_FALSE, _stride, _normalOffset);
		}

		if (_shader->get_aTextCoords() != -1) {
			glEnableVertexAttribArray(_shader->get_aTextCoords());
			glVertexAttribPointer(_shader->get_aTextCoords(), 2, GL_FLOAT, GL_FALSE, _stride, _textureCoordOffset);
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	else {
		glGenBuffers(1, &_vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
}

VertexBuffer::VertexBuffer(const GLvoid *data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset, boolean indicesWith, const char* s) : _mode(mode), _count(count), _stride(stride), _shader(shader), _shaderData(shaderData), _positionOffset(positionOffset), _normalOffset(normalOffset), _textureCoordOffset(textureCoordOffset)
{

	_vexterArrayID = -1;
	_amount = 1;
	if (indicesWith == true)
	{
		glGenVertexArrays(1, &_vexterArrayID);
		glGenBuffers(1, &_vertexBufferID);
		glGenBuffers(1, &_elementBufferObjectID);

		glBindVertexArray(_vexterArrayID);

		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObjectID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		if (_shader->get_aPositionVertex() != -1) {
			glEnableVertexAttribArray(_shader->get_aPositionVertex());
			glVertexAttribPointer(_shader->get_aPositionVertex(), 3, GL_FLOAT, GL_FALSE, _stride, _positionOffset);
		}

		if (_shader->get_aPositionNormals() != -1) {
			glEnableVertexAttribArray(_shader->get_aPositionNormals());
			glVertexAttribPointer(_shader->get_aPositionNormals(), 3, GL_FLOAT, GL_FALSE, _stride, _normalOffset);
		}

		if (_shader->get_aTextCoords() != -1) {
			glEnableVertexAttribArray(_shader->get_aTextCoords());
			glVertexAttribPointer(_shader->get_aTextCoords(), 2, GL_FLOAT, GL_FALSE, _stride, _textureCoordOffset);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	else {
		glGenBuffers(1, &_vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

VertexBuffer::VertexBuffer(std::vector<VertexDataPNT> data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset, std::vector<GLuint> indices) : _mode(mode), _count(count), _stride(stride), _shader(shader), _shaderData(shaderData), _positionOffset(positionOffset), _normalOffset(normalOffset), _textureCoordOffset(textureCoordOffset)
{
	_vexterArrayID = -1;

	_amount = 1;
		glGenVertexArrays(1, &_vexterArrayID);
		glGenBuffers(1, &_vertexBufferID);
		glGenBuffers(1, &_elementBufferObjectID);

		glBindVertexArray(_vexterArrayID);

		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, &data.front(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObjectID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		if (_shader->get_aPositionVertex() != -1) {
			glEnableVertexAttribArray(_shader->get_aPositionVertex());
			glVertexAttribPointer(_shader->get_aPositionVertex(), 3, GL_FLOAT, GL_FALSE, _stride, _positionOffset);
		}

		if (_shader->get_aPositionNormals() != -1) {
			glEnableVertexAttribArray(_shader->get_aPositionNormals());
			glVertexAttribPointer(_shader->get_aPositionNormals(), 3, GL_FLOAT, GL_FALSE, _stride, _normalOffset);
		}

		if (_shader->get_aTextCoords() != -1) {
			glEnableVertexAttribArray(_shader->get_aTextCoords());
			glVertexAttribPointer(_shader->get_aTextCoords(), 2, GL_FLOAT, GL_FALSE, _stride, _textureCoordOffset);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	
}

VertexBuffer::VertexBuffer(std::vector<VertexDataPNT> data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset, std::vector<GLuint> indices, boolean instancing, GLuint amount) : _mode(mode), _count(count), _stride(stride), _shader(shader), _shaderData(shaderData), _positionOffset(positionOffset), _normalOffset(normalOffset), _textureCoordOffset(textureCoordOffset), _amount(amount)
{
	_vexterArrayID = -1;
	
	//glm::mat4* modelMatrices;
	//glm::mat4 ModelMatrices[50000];

	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];

	//rand(glfwGetTime());
	//srand(glfwGetTime()); // initialize random seed	
	GLfloat radius = 65.0;
	GLfloat offset = 20.5f;
	for (GLuint i = 0; i < amount; i++)
	{
		glm::mat4 model;
		// 1. Translation: displace along circle with 'radius' in range [-offset, offset]
		GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f;
		GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat x = sin(angle) * radius + displacement;
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat y = displacement * 0.5f; // Keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat z = cos(angle) * radius + displacement;

		model = glm::translate(model, glm::vec3(x, y, z));
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		// 2. Scale: Scale between 0.05 and 0.25f
		GLfloat scale = (rand() % 20) / 100.0f + 0.05;
		//GLfloat scale = 10.0f;
		model = glm::scale(model, glm::vec3(scale));

		// 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
		GLfloat rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. Now add to list of matrices
		modelMatrices[i] = model;
	}

	glGenVertexArrays(1, &_vexterArrayID);
	glGenBuffers(1, &_vertexBufferID);
	glGenBuffers(1, &_elementBufferObjectID);

	glBindVertexArray(_vexterArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, size, &data.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObjectID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	if (_shader->get_aPositionVertex() != -1) {
		glEnableVertexAttribArray(_shader->get_aPositionVertex());
		glVertexAttribPointer(_shader->get_aPositionVertex(), 3, GL_FLOAT, GL_FALSE, _stride, _positionOffset);
	}

	if (_shader->get_aPositionNormals() != -1) {
		glEnableVertexAttribArray(_shader->get_aPositionNormals());
		glVertexAttribPointer(_shader->get_aPositionNormals(), 3, GL_FLOAT, GL_FALSE, _stride, _normalOffset);
	}

	if (_shader->get_aTextCoords() != -1) {
		glEnableVertexAttribArray(_shader->get_aTextCoords());
		glVertexAttribPointer(_shader->get_aTextCoords(), 2, GL_FLOAT, GL_FALSE, _stride, _textureCoordOffset);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLuint VAO = _vexterArrayID;
	GLuint buffer;
	glBindVertexArray(VAO);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
	// Set attribute pointers for matrix (4 times vec4)
	GLsizei vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &_vertexBufferID);
	_vertexBufferID = 0;
}

GLuint VertexBuffer::getVertexBuffer()
{
	return _vertexBufferID;
}

void VertexBuffer::renderVertexBuffer()
{
	if (_amount > 1)
	{
		glDrawElementsInstanced(GL_TRIANGLES, _count, GL_UNSIGNED_INT, 0, _amount);
	}
	else {
		glDrawElements(GL_TRIANGLES, _count, GL_UNSIGNED_INT, 0);
	}
	
	
	if (_vexterArrayID != -1) {
		glBindVertexArray(0);
	}
	
}

void VertexBuffer::configureVertexAttributes()
{
	if (_vexterArrayID != -1) {
		glBindVertexArray(_vexterArrayID);
	}

	// Directional light
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "dirLight.direction"), 0.2f, 1.0f, 3.3f);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "dirLight.ambient"), 0.5f, 0.5f,0.5f);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "dirLight.specular"), 0.5f, 0.5f, 0.5f);
	
	
}

ShaderInterface* VertexBuffer::get_Shader()
{
	return _shader;
}

ShaderData* VertexBuffer::get_shaderData()
{
	return _shaderData;
}

void VertexBuffer::renderMaterials()
{
	glActiveTexture(GL_TEXTURE0);
	//glActiveTexture(GL_TEXTURE1);
	//glActiveTexture(GL_TEXTURE2);
	//glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, _materials->at(0)->get_textureID());
    //glBindTexture(GL_TEXTURE_2D, _materials->at(0)->get_textureID_2());

	/*GLint matDiffuseLoc = glGetUniformLocation(_shader->getProgramHandle(), "material.diffuse");
	GLint matSpecularLoc = glGetUniformLocation(_shader->getProgramHandle(), "material.specular");
	GLint matShineLoc = glGetUniformLocation(_shader->getProgramHandle(), "material.shininess");

	glUniform1i(matDiffuseLoc, _materials->at(0)->get_textureID());
	glUniform1i(matSpecularLoc, _materials->at(0)->get_textureID_2());
	glUniform1f(matShineLoc, _materials->at(0)->get_shineness());*/

	GLint matAmbientLoc = glGetUniformLocation(_shader->getProgramHandle(), "material.ambient");
	GLint matDiffuseLoc = glGetUniformLocation(_shader->getProgramHandle(), "material.diffuse");
	GLint matSpecularLoc = glGetUniformLocation(_shader->getProgramHandle(), "material.specular");
	GLint matShineLoc = glGetUniformLocation(_shader->getProgramHandle(), "material.shininess");

	glUniform3f(matAmbientLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(matDiffuseLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
	glUniform1f(matShineLoc, 32.0f);
}

std::vector<materials*>* VertexBuffer::get_materials()
{
	return _materials;
}

void VertexBuffer::set_materials(std::vector<materials*>*materials)
{
	_materials = materials;
}

void VertexBuffer::generateAmount() {

	//glm::mat4* modelMatrices;
	//modelMatrices = new glm::mat4[1000];
	
}

GLuint VertexBuffer::get_amount()
{
	return _amount;
}