#include "stdafx.h"
#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(const GLvoid *data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset) : _mode(mode), _count(count), _stride(stride), _shader(shader), _shaderData(shaderData), _positionOffset(positionOffset), _normalOffset(normalOffset), _textureCoordOffset(textureCoordOffset)
{
	glGenBuffers(1, &_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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
	glDrawArrays(_mode,0,_count);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::configureVertexAttributes()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);

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
		//glm::mat4 m_modelViewMatrix = glm::lookAt(...);*/
		//m_modelViewMatrix *= glm::translate(...) * glm::rotate(...) *(etc.);*/

		glm::mat4 m_projectionMatrix = glm::perspective(75.0f, (1280.0f / 720.0f), 0.1f, 1000.0f);
		GLuint transformLoc = glGetUniformLocation(_shader->getProgramHandle(), "projectionMatrix");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));

		glm::mat4 m_modelMatrix;

		m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
		GLuint transformLoc2 = glGetUniformLocation(_shader->getProgramHandle(), "modelMatrix");
		glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));

		glm::mat4 m_ViewMatrix;

		m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		GLuint transformLoc3 = glGetUniformLocation(_shader->getProgramHandle(), "viewMatrix");
		glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(m_ViewMatrix));
	}
}

ShaderInterface* VertexBuffer::get_Shader()
{
	return _shader;
}

ShaderData* VertexBuffer::get_shaderData()
{
	return _shaderData;
}