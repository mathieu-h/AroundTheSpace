#pragma once
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "opengl32.lib")
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include "ShaderInterface.h"
#include "ShaderData.h"
#include "materials.h"
#include "VertexDataP.h"
#include <vector>
//#include "Text.h"

#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class VertexBuffer
{
private:
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

	GLuint _amount;

public:
	GLuint getVertexBuffer();
	ShaderInterface* get_Shader();
	ShaderData* get_shaderData();

	std::vector<materials*>*_materials;

	GLuint _cubemaptexture = -1;
	bool _cube;

	VertexBuffer(const GLvoid *data, GLsizeiptr size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid*positionOffset, GLvoid *normalOffset, GLvoid* textureCoordOffset, boolean indicesWith);
	VertexBuffer(const GLvoid *data, GLsizeiptr size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid*positionOffset, GLvoid *normalOffset, GLvoid* textureCoordOffset, boolean indicesWith, const char* s);
	VertexBuffer(const GLvoid *data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset, std::vector<GLuint> indices, GLuint amount, ShaderInterface* _shaderdepth);

	VertexBuffer(std::vector<VertexDataPNT> data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset, std::vector<GLuint> indices, Vector3 _originPos, float _timeOffset, float _speed, bool _isStar, GLuint amount, vector<Vector3> arrayPos);

	VertexBuffer(std::vector<VertexDataPNT> data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset, std::vector<GLuint> indices, Vector3 _originPos, float _timeOffset, float _speed, bool _isStar);
	VertexBuffer(std::vector<VertexDataPNT> data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset, std::vector<GLuint> indices, GLuint amount);
	VertexBuffer(std::vector<VertexDataPNT> data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset, std::vector<GLuint> indices, boolean instancing, GLuint amount);
	VertexBuffer(std::vector<VertexDataPNT> data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset, std::vector<GLuint> indices);

	VertexBuffer(const GLvoid *data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset);
	VertexBuffer(const GLvoid *data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset);
	VertexBuffer(const GLvoid *data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* normalOffset, GLvoid* textureCoordOffset, int amount);
	VertexBuffer(const GLvoid *data, GLsizei size, GLenum mode, GLsizei count, GLsizei stride, ShaderInterface* shader, ShaderData* shaderData, GLvoid* positionOffset, GLvoid* textureCoordOffset);
	~VertexBuffer();

	void configureVertexAttributes();
	void renderVertexBuffer();

	void renderMaterials();
	void generateAmount();

	std::vector<materials*>* get_materials();
	void set_materials(std::vector<materials*>*materials);
	GLuint get_amount();

	GLuint loadCubemap(vector<const GLchar*> faces);

	ShaderInterface* _shaderDepthFramed;
	ShaderInterface* _shaderDepth;

	Vector3 originPos;

	float timeOffset = 0.0f;
	float speed;
	bool isStart;

	Vector3 middle;
};

