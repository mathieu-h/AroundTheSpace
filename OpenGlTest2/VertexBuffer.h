#pragma once
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "opengl32.lib")
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include "ShaderInterface.h"
#include "ShaderData.h"
#include "Planet.h"
#include "materials.h"

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

	ShaderInterface* _shader;
	ShaderData *_shaderData;

	GLenum _mode;
	GLsizei _count;
	GLsizei _stride;
	GLvoid *_positionOffset;
	GLvoid *_normalOffset;
	GLvoid *_textureCoordOffset;

	std::vector<materials*>*_materials;
	GLuint _amount;
	
	//glm::mat4* modelMatrices;

public:
	GLuint getVertexBuffer();
	ShaderInterface* get_Shader();
	ShaderData* get_shaderData();

	

	VertexBuffer(const GLvoid *data, GLsizeiptr size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid*positionOffset, GLvoid *normalOffset, GLvoid* textureCoordOffset, boolean indicesWith);
	VertexBuffer(const GLvoid *data, GLsizeiptr size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid*positionOffset, GLvoid *normalOffset, GLvoid* textureCoordOffset, boolean indicesWith, const char* s);
	VertexBuffer(std::vector<VertexDataPNT> data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset, std::vector<GLuint> indices);
	VertexBuffer(std::vector<VertexDataPNT> data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset, std::vector<GLuint> indices, boolean instancing, GLuint amount);
	~VertexBuffer();

	void configureVertexAttributes();
	void renderVertexBuffer();

	void renderMaterials();
	void generateAmount();

	std::vector<materials*>* get_materials();
	void set_materials(std::vector<materials*>*materials);
	GLuint get_amount();
};

