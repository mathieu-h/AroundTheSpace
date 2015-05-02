#pragma once
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "opengl32.lib")
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include "ShaderInterface.h"
#include "ShaderData.h"

#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class VertexBuffer
{
private :
	GLuint _vertexBufferID;
	GLuint _vexterArrayID;
	GLuint _elementBufferObjectID;
	GLuint _textureID;
	ShaderInterface* _shader;
	ShaderData *_shaderData;

	GLenum _mode;
	GLsizei _count;
	GLsizei _stride;
	GLvoid *_positionOffset;
	GLvoid *_normalOffset;
	GLvoid *_textureCoordOffset;

public:
	GLuint getVertexBuffer();
	ShaderInterface* get_Shader();
	ShaderData* get_shaderData();

	VertexBuffer(const GLvoid *data, GLsizeiptr size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid*positionOffset, GLvoid *normalOffset, GLvoid* textureCoordOffset, boolean indicesWith);
	VertexBuffer(const GLvoid *data, GLsizeiptr size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid*positionOffset, GLvoid *normalOffset, GLvoid* textureCoordOffset, boolean indicesWith, const char* s);
	~VertexBuffer();

	void configureVertexAttributes();
	void renderVertexBuffer();
};

