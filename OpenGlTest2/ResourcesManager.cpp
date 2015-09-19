#include "stdafx.h"
#include "ResourcesManager.h"
#include "TriangleVertices.h"
#include "CubeVertices.h"
#include "verticesTiangle.h"
#include "CubeQuadTex.h"
#include "Planet.h"
#include "model.h"
#include "Star.h"
#include "PlanetarySystem.h"

ResourcesManager::ResourcesManager()
{
	_shaderArray = new std::vector < ShaderInterface* >;

	ShaderInterface *textureShader = new ShaderInterface("../shaders/VertexShaderTexture.vs", "../shaders/FragmentShaderTexture.fs");
	_shaderArray->push_back(textureShader);

	ShaderInterface *instancingShader = new ShaderInterface("../shaders/instancing.vs", "../shaders/instancing.frag");
	_shaderArray->push_back(instancingShader);

	ShaderInterface *lightShader = new ShaderInterface("../shaders/light.vs", "../shaders/light.frag");
	_shaderArray->push_back(lightShader);

	ShaderInterface *textShader = new ShaderInterface("../shaders/TextRender.vs", "../shaders/TextRender.frag");
	_shaderArray->push_back(textShader);

	ShaderInterface* skyboxShader = new ShaderInterface("../shaders/skybox.vs", "../shaders/skybox.frag");
	_shaderArray->push_back(skyboxShader);

	ShaderInterface* depthShader = new ShaderInterface("../shaders/shadow_mapping_depth.vs", "../shaders/shadow_mapping_depth.frag");
	_shaderArray->push_back(depthShader);

	ShaderInterface* depthDebugShader = new ShaderInterface("../shaders/depthMap.vs", "../shaders/depthMap.frag");
	_shaderArray->push_back(depthDebugShader);

	ShaderInterface* depthShaderinstancing = new ShaderInterface("../shaders/shadow_mapping_depth_instancing.vs", "../shaders/shadow_mapping_depth_instancing.frag");
	_shaderArray->push_back(depthShaderinstancing);

	ShaderInterface* depthDebugShaderinstancing = new ShaderInterface("../shaders/depthMap_instancing.vs", "../shaders/depthMap_instancing.frag");
	_shaderArray->push_back(depthDebugShaderinstancing);

	ShaderInterface *textureStarShader = new ShaderInterface("../shaders/VertexStarShaderTexture.vs", "../shaders/FragmentStarShaderTexture.fs");
	_shaderArray->push_back(textureStarShader);

	ShaderInterface *textureStarShaderInstancing = new ShaderInterface("../shaders/VertexStarShaderInstancingTexture.vs", "../shaders/FragmentStarShaderInstancingTexture.fs");
	_shaderArray->push_back(textureStarShaderInstancing);

	ShaderInterface *textureArrow = new ShaderInterface("../shaders/ArrowRender.vs", "../shaders/ArrowRender.frag");
	_shaderArray->push_back(textureArrow);

	_shaderData = new ShaderData(makeVector4(1.0f, 0.0f, 1.0f, 1.0f), makeVector3(1.0f, 0.0f, 1.0f));


	_vertexBufferArray = new std::vector<VertexBuffer*>();

	//Planet p;

	//Star s;

	//PlanetarySystem pS(makeVector3(0.0f,0.f,-1000.0f));
	
	//_star.push_back(pS.star);

	//std::vector<GameObject> _arrayGameObjects = pS.GetGameObject();

	std::vector<std::vector<materials*>*> _arrayMaterials = std::vector<std::vector<materials*>*>();

	/*for (int i = 0; i < _arrayGameObjects.size(); i++)
	{
		GameObject g = _arrayGameObjects.at(i);
		VertexBuffer* _planetVertexBuffer;
		if (i == 0)
		{
			_planetVertexBuffer = new VertexBuffer(g.Vnu, g.Vnu.size() * sizeof(g.Vnu) * 2, GL_TRIANGLES, g.triangles.size(), sizeof(VertexDataPNT), _shaderArray->at(9), _shaderData, (GLvoid*)(offsetof(VertexDataPNT, positionCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, normalCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, textureCoordinates)), g.triangles, pS.startPos, g.timeOffset, g.speed, true);
		} else{
			_planetVertexBuffer = new VertexBuffer(g.Vnu, g.Vnu.size() * sizeof(g.Vnu) * 2, GL_TRIANGLES, g.triangles.size(), sizeof(VertexDataPNT), _shaderArray->at(0), _shaderData, (GLvoid*)(offsetof(VertexDataPNT, positionCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, normalCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, textureCoordinates)), g.triangles, pS.startPos, g.timeOffset, g.speed, false);
		}
		_arrayMaterials.push_back(new std::vector<materials*>());
		_arrayMaterials.at(i)->push_back(g.mat);
		_planetVertexBuffer->set_materials(_arrayMaterials.at(i));
		_vertexBufferArray->push_back(_planetVertexBuffer);
		_planets.push_back(g);
	}*/

	VertexBuffer* _vertexBufftext3 = new VertexBuffer(skyboxVertices, sizeof(skyboxVertices), GL_TRIANGLES, 36, sizeof(VertexDataP), _shaderArray->at(4), _shaderData, (GLvoid*)(offsetof(VertexDataP, positionCoordinates)));
	_vertexBufferArray->push_back(_vertexBufftext3);
	std::vector<materials*> *_materials2 = new std::vector<materials*>();

	/*VertexBuffer* _planetVertexBuffer = new VertexBuffer(s.Vnu, s.Vnu.size() * sizeof(s.Vnu) * 2, GL_TRIANGLES, s.triangles.size(), sizeof(VertexDataPNT), _shaderArray->at(0), _shaderData, (GLvoid*)(offsetof(VertexDataPNT, positionCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, normalCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, textureCoordinates)), s.triangles);
	_planetVertexBuffer->set_materials(_materials2);*/
	//_vertexBufferArray->push_back(_planetVertexBuffer);

	Model m("objects/arrow.obj", _shaderArray->at(11));
	///m.meshes.at(0)->set_materials(_materials);
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
