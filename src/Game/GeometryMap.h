#pragma once

#include "Vertex.h"

constexpr unsigned AreaSizeXZ = 40;

enum class TileType : uint8_t
{
	None, // не рисуетс€
	Solid,
	// рампы разных высот
	RampL,
	RampR,
	RampT,
	RampB,

	// лестницы

	Max
};

struct Tile3D
{
	Tile3D& operator=(const Tile3D&) = default;

	TileType type = TileType::None;

	uint8_t textureTopId = 0;
	uint8_t textureWallId = 0;

	/* -4 = -1.0
		0 - лини€ мор€
		1 = 0.25
		2 = 0.5
		3 = 0.75
		4 = 1.0
		8 = 2.0*/
	int8_t posY;
};

struct Area
{
	Tile3D tiles[AreaSizeXZ][AreaSizeXZ];
};

constexpr float s = sizeof(Area) / 1024.0 / 1024.0;


struct GeometryMap
{
	void Create(ShaderProgramRef shader);

	size_t AddGeomBuff(ShaderProgramRef shader);

	void Reset()
	{
		for (size_t i = 0; i < arr.size(); i++)
		{
			arr[i].geom.reset();
			arr[i].vertices.clear();
			arr[i].indices.clear();
			arr[i].numIndex = 0;
			arr[i].textureId = 0;
		}
		arr.clear();
	}

	struct DataArr
	{
		GeometryBufferRef geom;
		std::vector<TileVertex> vertices;
		std::vector<unsigned> indices;
		unsigned numIndex = 0;
		uint8_t textureId = 0;
	};

	std::vector<DataArr> arr;
};