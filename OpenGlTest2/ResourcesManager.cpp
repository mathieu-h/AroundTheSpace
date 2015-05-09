#include "stdafx.h"
#include "ResourcesManager.h"
#include "TriangleVertices.h"
#include "CubeVertices.h"
#include "verticesTiangle.h"
#include "CubeQuadTex.h"
#include "Planet.h"

ResourcesManager::ResourcesManager()
{
	_shaderArray = new std::vector < ShaderInterface* >;
	/*ShaderInterface *shader = new ShaderInterface("VertexShader.fs", "ColorShader.fs");
	_shaderArray->push_back(shader);

	ShaderInterface *lightShader = new ShaderInterface("SimpleLightShader.vsh", "SimpleLightShader.fsh");
	_shaderArray->push_back(lightShader);*/

	ShaderInterface *textureShader = new ShaderInterface("VertexShaderTexture.vs", "FragmentShaderTexture.fs");
	_shaderArray->push_back(textureShader);

	_shaderData = new ShaderData(makeVector4(1.0f, 0.0f, 1.0f, 1.0f), makeVector3(1.0f, 0.0f, 1.0f));


	_vertexBufferArray = new std::vector<VertexBuffer*>();

	VertexBuffer* _vertexBufftext2 = new VertexBuffer(verticesCubeWithTexture, sizeof(verticesCubeWithTexture), GL_TRIANGLES, 36, sizeof(VertexDataPNT), _shaderArray->at(0), _shaderData, (GLvoid*)(offsetof(VertexDataPNT, positionCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, normalCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, textureCoordinates)), true, "SplatterLong0079_S.jpg");
	_vertexBufferArray->push_back(_vertexBufftext2);

	VertexBuffer* _vertexBufftext3 = new VertexBuffer(verticesCubeWithTexture, sizeof(verticesCubeWithTexture), GL_TRIANGLES, 36, sizeof(VertexDataPNT), _shaderArray->at(0), _shaderData, (GLvoid*)(offsetof(VertexDataPNT, positionCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, normalCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, textureCoordinates)), true, "OrnamentsHeads0036_S.jpg");
	_vertexBufferArray->push_back(_vertexBufftext3);

	Planet p;

	VertexBuffer* _planetVertexBuffer = new VertexBuffer(p.Vnu, p.Vnu.size() * sizeof(p.Vnu), GL_TRIANGLES, p.triangles.size(), sizeof(VertexDataPNT), _shaderArray->at(0), _shaderData, (GLvoid*)(offsetof(VertexDataPNT, positionCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, normalCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, textureCoordinates)), p.triangles, "Gas Giant-Blue.jpg");
	_vertexBufferArray->push_back(_planetVertexBuffer);

	/*VertexBuffer* _vertexBufftext = new VertexBuffer(p.vertices, sizeof(verticesCubeWithTexture), GL_TRIANGLES, 36, sizeof(VertexDataPNT), _shaderArray->at(0), _shaderData, (GLvoid*)(offsetof(VertexDataPNT, positionCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, normalCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, textureCoordinates)), true, "OrnamentsHeads0036_S.jpg");
	_vertexBufferArray->push_back(_vertexBufftext);*/
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
