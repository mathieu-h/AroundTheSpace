#include "stdafx.h"
#include "ResourcesManager.h"
#include "TriangleVertices.h"
#include "CubeVertices.h"
#include "verticesTiangle.h"
#include "CubeQuadTex.h"
#include "Planet.h"
#include "model.h"

ResourcesManager::ResourcesManager()
{
	_shaderArray = new std::vector < ShaderInterface* >;
	/*ShaderInterface *shader = new ShaderInterface("VertexShader.fs", "ColorShader.fs");
	_shaderArray->push_back(shader);*/

	

	ShaderInterface *textureShader = new ShaderInterface("VertexShaderTexture.vs", "FragmentShaderTexture.fs");
	_shaderArray->push_back(textureShader);

	ShaderInterface *instancingShader = new ShaderInterface("instancing.vs", "instancing.frag");
	_shaderArray->push_back(instancingShader);

	_shaderData = new ShaderData(makeVector4(1.0f, 0.0f, 1.0f, 1.0f), makeVector3(1.0f, 0.0f, 1.0f));


	_vertexBufferArray = new std::vector<VertexBuffer*>();

	VertexBuffer* _vertexBufftext2 = new VertexBuffer(verticesCubeWithTexture, sizeof(verticesCubeWithTexture), GL_TRIANGLES, 36, sizeof(VertexDataPNT), _shaderArray->at(0), _shaderData, (GLvoid*)(offsetof(VertexDataPNT, positionCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, normalCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, textureCoordinates)), true, "SplatterLong0079_S.jpg");
	_vertexBufferArray->push_back(_vertexBufftext2);

	VertexBuffer* _vertexBufftext3 = new VertexBuffer(verticesCubeWithTexture, sizeof(verticesCubeWithTexture), GL_TRIANGLES, 36, sizeof(VertexDataPNT), _shaderArray->at(0), _shaderData, (GLvoid*)(offsetof(VertexDataPNT, positionCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, normalCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, textureCoordinates)), true, "OrnamentsHeads0036_S.jpg");
	_vertexBufferArray->push_back(_vertexBufftext3);

	Planet p;

	std::vector<materials*> *_materials = new std::vector<materials*>();
	_materials->push_back(new materials("Rock-Texture-Surface.jpg", "Rock-Texture-Surface.jpg", 32.0f));
	std::vector<materials*> *_materials2 = new std::vector<materials*>();
	_materials2->push_back(new materials("Gas Giant-Blue.jpg", "Gas Giant-Blue.jpg", 32.0f));

	VertexBuffer* _planetVertexBuffer = new VertexBuffer(p.Vnu, p.Vnu.size() * sizeof(p.Vnu) * 2, GL_TRIANGLES, p.triangles.size(), sizeof(VertexDataPNT), _shaderArray->at(0), _shaderData, (GLvoid*)(offsetof(VertexDataPNT, positionCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, normalCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, textureCoordinates)), p.triangles);
	_planetVertexBuffer->set_materials(_materials2);
	_vertexBufferArray->push_back(_planetVertexBuffer);

	Model m("rock.obj", _shaderArray->at(1));
	m.meshes.at(0)->set_materials(_materials);
	_vertexBufferArray->push_back(m.meshes.at(0));

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
