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
	
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

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