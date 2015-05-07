#include "stdafx.h"
#include "Planet.h"

using namespace noise;



Planet::Planet()
{
	this->generatePlanet();
}



Planet::~Planet()
{
}



void Planet::generatePlanet()
{
	float radius = 1.0f;
	const int nbLong = 24;
	const int nbLat = 16;
	const int nbVertices = (nbLong + 1) * nbLat + 2;
	Vector3 vector3Up = makeVector3(0.0f, 1.0f, 0.0f);

#pragma region Vertices
	vertices = std::vector<Vector3>(nbVertices);
	const float _pi = M_PI;
	const float _2pi = _pi * 2.0f;

	vertices[0] = scalerMultiplyVector3(vector3Up, radius);
	for (int lat = 0; lat < nbLat; ++lat)
	{
		float a1 = _pi * float(lat + 1) / (nbLat + 1);
		float sin1 = sin(a1);
		float cos1 = cos(a1);
		
		for (int lon = 0; lon <= nbLong; ++lon)
		{
			float a2 = _2pi * float(lon == nbLong ? 0 : lon) / nbLong;
			float sin2 = sin(a2);
			float cos2 = cos(a2);

			vertices[lon + lat * (nbLong + 1) + 1] = scalerMultiplyVector3(makeVector3(sin1 * cos2, cos1, sin1 * sin2), radius);
		}
	}
	vertices[nbVertices - 1] = scalerMultiplyVector3(vector3Up, -radius);
#pragma endregion

#pragma region Normales
	normales = std::vector<Vector3>(nbVertices);
	for (int n = 0; n < nbVertices; n++)
		normales[n] = normalizeVector3(vertices[n]);
#pragma endregion

#pragma region UVs
	uvs = std::vector<Vector2>(nbVertices);
	uvs[0] = makeVector2(1.0f, 1.0f);
	uvs[nbVertices - 1] = makeVector2(0.0f, 0.0f);
	for (int lat = 0; lat < nbLat; lat++)
		for (int lon = 0; lon <= nbLong; lon++)
			uvs[lon + lat * (nbLong + 1) + 1] = makeVector2((float)lon / nbLong, 1.0f - (float)(lat + 1) / (nbLat + 1));
#pragma endregion

#pragma region Triangles
	const int nbFaces = nbVertices;
	const int nbTriangles = nbFaces * 2;
	const int nbIndexes = nbTriangles * 3;
	triangles = std::vector<int>(nbIndexes);

	//Top Cap
	int i = 0;
	for (int lon = 0; lon < nbLong; ++lon)
	{
		triangles[i++] = lon + 2;
		triangles[i++] = lon + 1;
		triangles[i++] = 0;
	}

	//Middle
	for (int lat = 0; lat < nbLat - 1; ++lat)
	{
		for (int lon = 0; lon < nbLong; ++lon)
		{
			int current = lon + lat * (nbLong + 1) + 1;
			int next = current + nbLong + 1;

			triangles[i++] = current;
			triangles[i++] = current + 1;
			triangles[i++] = next + 1;

			triangles[i++] = current;
			triangles[i++] = next + 1;
			triangles[i++] = next;
		}
	}

	//Bottom Cap
	for (int lon = 0; lon < nbLong; ++lon)
	{
		triangles[i++] = nbVertices - 1;
		triangles[i++] = nbVertices - (lon + 2) - 1;
		triangles[i++] = nbVertices - (lon + 1) - 1;
	}
#pragma endregion
}



void Planet::generateHeightMap()
{

}
