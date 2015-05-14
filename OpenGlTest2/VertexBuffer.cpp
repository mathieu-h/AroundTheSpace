#include "stdafx.h"
#include "VertexBuffer.h"
#include "indices.h"

VertexBuffer::VertexBuffer(const GLvoid *data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset, boolean indicesWith) : _mode(mode), _count(count), _stride(stride), _shader(shader), _shaderData(shaderData), _positionOffset(positionOffset), _normalOffset(normalOffset), _textureCoordOffset(textureCoordOffset)
{
	
	_vexterArrayID = -1;

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

	
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(s, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

VertexBuffer::VertexBuffer(std::vector<VertexDataPNT> data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset, std::vector<GLuint> indices, const char* s) : _mode(mode), _count(count), _stride(stride), _shader(shader), _shaderData(shaderData), _positionOffset(positionOffset), _normalOffset(normalOffset), _textureCoordOffset(textureCoordOffset)
{
	_vexterArrayID = -1;


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

	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(s, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
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

	//glDrawArrays(_mode, 0, _count);
	
	glDrawElements(GL_TRIANGLES, _count, GL_UNSIGNED_INT, 0);

	if (_vexterArrayID != -1) {
		glBindVertexArray(0);
	}
	
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::configureVertexAttributes()
{
	if (_vexterArrayID != -1) {
		glBindVertexArray(_vexterArrayID);
	}
	
	if (_textureID != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _textureID);
		glUniform1i(glGetUniformLocation(_shader->getProgramHandle(), "material.diffuse"), 0);
	}

	GLint matDiffuseLoc = glGetUniformLocation(_shader->getProgramHandle(), "material.diffuse");
	GLint matSpecularLoc = glGetUniformLocation(_shader->getProgramHandle(), "material.specular");
	GLint matShineLoc = glGetUniformLocation(_shader->getProgramHandle(), "material.shininess");

	glUniform3f(matDiffuseLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
	glUniform1f(matShineLoc, 32.0f);

	/*GLint lightAmbientLoc = glGetUniformLocation(_shader->getProgramHandle(), "light.ambient");
	GLint lightDiffuseLoc = glGetUniformLocation(_shader->getProgramHandle(), "light.diffuse");
	GLint lightSpecularLoc = glGetUniformLocation(_shader->getProgramHandle(), "light.specular");

	glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
	glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f); // Let's darken the light a bit to fit the scene
	glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

	glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "light.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "light.linear"), 0.09);
	glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "light.quadratic"), 0.032);*/

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};

	// Directional light
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "dirLight.specular"), 0.5f, 0.5f, 0.5f);
	// Point light 1
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[0].linear"), 0.09);
	glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[0].quadratic"), 0.032);
	// Point light 2
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[1].linear"), 0.09);
	glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[1].quadratic"), 0.032);
	// Point light 3
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[2].linear"), 0.09);
	glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[2].quadratic"), 0.032);
	// Point light 4
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[3].linear"), 0.09);
	glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[3].quadratic"), 0.032);
	
	//glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	
	
	
}

ShaderInterface* VertexBuffer::get_Shader()
{
	return _shader;
}

ShaderData* VertexBuffer::get_shaderData()
{
	return _shaderData;
}