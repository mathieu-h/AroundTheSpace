#include "stdafx.h"
#include "Star.h"


Star::Star()
{
	this->generateStar();
	worldPosition = makeVector3(0.0f, 0.0f, 0.0f);
}


Star::~Star()
{
}


void Star::generateStar()
{
	radius = 90.0f + rand() % 15;

	int c = rand() % 3;

	compositionColor1.red = c == 0 ? 255 : rand() % 75 + 175;
	compositionColor1.green = c == 1 ? 255 : rand() % 75 + 175;
	compositionColor1.blue = c == 2 ? 255 : rand() % 75 + 175;
	compositionColor1.alpha = 255;

	compositionColor2.red = c == 0 ? 255 : rand() % 75 + 175;
	compositionColor2.green = c == 1 ? 255 : rand() % 75 + 175;
	compositionColor2.blue = c == 2 ? 255 : rand() % 75 + 175;
	compositionColor2.alpha = 255;

	utils::NoiseMap heightMap = generateHeightMap();
	int heightMapHeight = heightMap.GetHeight();
	int heightMapWidth = heightMap.GetWidth();

	generateTexture(heightMap);

	vector<Vector3> vertices;
	vector<Vector3> normales;
	vector<Vector2> uvs;

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
			int heightX = int(float(lon) / (nbLong)* heightMapWidth);
			int heightY = int(float(lat) / (nbLat)* heightMapHeight);
			if (lon == nbLong)
				heightX = 0;
			//float height = heightMap.GetValue(heightX, heightY) * radius * 0.05f;
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
	triangles = std::vector<GLuint>(nbIndexes);

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

	Vnu = std::vector<VertexDataPNT>(nbVertices);

	for (int i = 0; i < nbVertices; ++i)
	{
		VertexDataPNT pnt;
		pnt.positionCoordinates = vertices[i];
		pnt.normalCoordinates = normales[i];
		pnt.textureCoordinates = uvs[i];
		Vnu[i] = pnt;
	}
#pragma endregion
}



utils::NoiseMap Star::generateHeightMap()
{
	module::RidgedMulti mountainTerrain;

	module::Billow baseFlatTerrain;
	baseFlatTerrain.SetFrequency(2.0);

	module::ScaleBias flatTerrain;
	flatTerrain.SetSourceModule(0, baseFlatTerrain);
	flatTerrain.SetScale(0.125);
	flatTerrain.SetBias(-0.75);

	module::Perlin terrainType;
	terrainType.SetFrequency(4);
	//terrainType.SetPersistence(0.25);
	terrainType.SetOctaveCount(6);
	terrainType.SetSeed(rand());

	module::Select terrainSelector;
	terrainSelector.SetSourceModule(0, flatTerrain);
	terrainSelector.SetSourceModule(1, mountainTerrain);
	terrainSelector.SetControlModule(terrainType);
	terrainSelector.SetBounds(0.0, 1000.0);
	terrainSelector.SetEdgeFalloff(2);

	module::Turbulence finalTerrain;
	finalTerrain.SetSourceModule(0, terrainSelector);
	finalTerrain.SetFrequency(8.0);
	finalTerrain.SetPower(0.125);

	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderSphere heightMapBuilder;
	heightMapBuilder.SetSourceModule(finalTerrain);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(2048, 1024);
	//heightMapBuilder.SetDestSize(1024, 512);
	heightMapBuilder.SetBounds(-90.0, 90.0, -180.0, 180.0);
	heightMapBuilder.Build();

	return heightMap;
}


void Star::generateTexture(utils::NoiseMap heightMap)
{
	utils::Image image;

	utils::RendererImage renderer;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.ClearGradient();

	renderer.AddGradientPoint(-1, compositionColor1);
	renderer.AddGradientPoint(1, compositionColor2);

	renderer.EnableLight();
	renderer.SetLightContrast(1.0);
	renderer.SetLightBrightness(2.0);
	renderer.Render();

	int heigth = heightMap.GetHeight();
	int width = heightMap.GetWidth();
	unsigned char* texture = new unsigned char[heigth * width * 3];
	int i = 0;
	for (int y = 0; y < heigth; ++y) {
		for (int x = 0; x < width; ++x) {
			utils::Color pixel = image.GetValue(x, y);
			texture[i++] = pixel.red;
			texture[i++] = pixel.green;
			texture[i++] = pixel.blue;
		}
	}

	mat = new materials(texture, width, heigth);
}