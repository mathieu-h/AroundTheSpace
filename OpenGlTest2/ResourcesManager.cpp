#include "stdafx.h"
#include "ResourcesManager.h"
#include "TriangleVertices.h"
#include "CubeVertices.h"
#include "verticesTiangle.h"
#include "CubeQuadTex.h"

ResourcesManager::ResourcesManager()
{
	_shaderArray = new std::vector < ShaderInterface* >;
	ShaderInterface *shader = new ShaderInterface("VertexShader.fs", "ColorShader.fs");
	_shaderArray->push_back(shader);

	ShaderInterface *lightShader = new ShaderInterface("SimpleLightShader.vsh", "SimpleLightShader.fsh");
	_shaderArray->push_back(lightShader);

	ShaderInterface *textureShader = new ShaderInterface("VertexShaderTexture.vs", "FragmentShaderTexture.fs");
	_shaderArray->push_back(textureShader);

	_shaderData = new ShaderData(makeVector4(1.0f, 0.0f, 1.0f, 1.0f), makeVector3(1.0f, 0.0f, 1.0f));


	_vertexBufferArray = new std::vector<VertexBuffer*>();
	/*VertexBuffer* _vertexBuff = new VertexBuffer(vertices, sizeof(vertices), GL_TRIANGLES, 3, sizeof(GLfloat) * 3, _shaderArray->at(0), _shaderData, NULL, NULL, NULL);
	_vertexBufferArray->push_back(_vertexBuff);*/

	VertexBuffer* _cubevertexBuff = new VertexBuffer(verticesCube, sizeof(verticesCube), GL_TRIANGLES, 36, sizeof(VertexDataPN), _shaderArray->at(1), _shaderData, (GLvoid*)(offsetof(VertexDataPN, positionCoordinates)), (GLvoid*)(offsetof(VertexDataPN, normalCoordinates)), NULL);
	_vertexBufferArray->push_back(_cubevertexBuff);


	VertexBuffer* _vertexBuffTriangle = new VertexBuffer(verticesTriangle, sizeof(verticesTriangle), GL_TRIANGLES, 12, sizeof(GLfloat) * 3, _shaderArray->at(0), _shaderData, NULL, NULL, NULL);
	_vertexBufferArray->push_back(_vertexBuffTriangle);

	/*VertexBuffer* _vertexBuffTriangle2 = new VertexBuffer(verticesTriangle, sizeof(verticesTriangle), GL_LINE_LOOP, 12, sizeof(GLfloat) * 3, _shaderArray->at(0), _shaderData, NULL, NULL, NULL);
	_vertexBufferArray->push_back(_vertexBuffTriangle2);*/

	VertexBuffer* _vertexBufftext = new VertexBuffer(verticesCubeQuad, sizeof(verticesCubeQuad), GL_TRIANGLES, 36, sizeof(VertexDataPT), _shaderArray->at(2), _shaderData, (GLvoid*)(offsetof(VertexDataPT, positionCoordinates)), NULL, (GLvoid*)(offsetof(VertexDataPT, textureCoordinates)));
	_vertexBufferArray->push_back(_vertexBufftext);

	VertexBuffer* _vertexBufftext = new VertexBuffer(verticesCubeQuad, sizeof(verticesCubeQuad), GL_LINE_LOOP, 36, sizeof(VertexDataPT), _shaderArray->at(2), _shaderData, (GLvoid*)(offsetof(VertexDataPT, positionCoordinates)), NULL, (GLvoid*)(offsetof(VertexDataPT, textureCoordinates)));
	_vertexBufferArray->push_back(_vertexBufftext);


	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image("SplatterLong0079_S.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
}



ResourcesManager::~ResourcesManager()
{
	for (std::vector<ShaderInterface* >::iterator it = _shaderArray->begin(); it != _shaderArray->end(); it++)
	{
		delete *it;
	}

	delete _shaderArray;

	for (std::vector<VertexBuffer* >::iterator it = _vertexBufferArray->begin(); it != _vertexBufferArray->end(); it++)
	{
		delete *it;
	}

	delete _shaderData;
	delete _vertexBufferArray;
}



std::vector<ShaderInterface*>* ResourcesManager::getShaderArray()
{
	return _shaderArray;
}

std::vector<VertexBuffer*>* ResourcesManager::getVertexBufferArray()
{
	return _vertexBufferArray;
}

ResourcesManager& ResourcesManager::getResourcesManager()
{
	static ResourcesManager * resourcesManager = NULL;

	if (resourcesManager == NULL)
	{
		resourcesManager = new ResourcesManager();
	}

	return *resourcesManager;
}

void ResourcesManager::destroyResourcesManager()
{
	ResourcesManager * resourcesManager = &getResourcesManager();
	delete resourcesManager;
}
