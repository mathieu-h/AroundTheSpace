#pragma once

#include <vector>
#include "Vector3.h"
#include "Vector2.h"
#include "VertexDataP.h"
#include "materials.h"


class GameObject
{
public:
	GameObject();
	~GameObject();

	std::vector<VertexDataPNT> Vnu;
	std::vector<GLuint> triangles;
	materials* mat;
	Vector3 worldPosition;

	float timeOffset;
	float speed;
};

