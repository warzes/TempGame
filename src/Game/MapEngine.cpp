#include "stdafx.h"
#include "MapEngine.h"
//-----------------------------------------------------------------------------
struct QuadDim
{
	QuadDim(Area* area, size_t x, size_t z)
	{
		x1 = -0.5f + x;
		x2 =  0.5f + x;
		y1 = -0.5f + area->tiles[x][z].posY / 4.0f;
		y2 =  0.5f + area->tiles[x][z].posY / 4.0f;
		z1 =  0.5f + z;
		z2 = -0.5f + z;

		int8_t raz1 = area->tiles[x][z].posY - area->tiles[x - 1][z].posY;
		razF1 = (x > 0 && raz1 > 0) ? (raz1 / 4.0f - 1.0f) : 0.0f;

		int8_t raz2 = area->tiles[x][z].posY - area->tiles[x + 1][z].posY;
		razF2 = ((x < AreaSizeXZ - 1) && raz2 > 0) ? (raz2 / 4.0f - 1.0f) : 0.0f;

		int8_t raz3 = area->tiles[x][z].posY - area->tiles[x][z - 1].posY;
		razF3 = (z > 0 && raz3 > 0) ? (raz3 / 4.0f - 1.0f) : 0.0f;

		int8_t raz4 = area->tiles[x][z].posY - area->tiles[x][z + 1].posY;
		razF4 = ((z < AreaSizeXZ - 1) && raz4 > 0) ? (raz4 / 4.0f - 1.0f) : 0.0f;
	}

	float x1;
	float x2;
	float y1;
	float y2;
	float z1;
	float z2;
	float razF1 = 0.0f;
	float razF2 = 0.0f;
	float razF3 = 0.0f;
	float razF4 = 0.0f;
};
//-----------------------------------------------------------------------------
void Map::SetTile(const Tile3D& tile, unsigned x, unsigned y)
{
	area->tiles[x][y] = tile;
}
//-----------------------------------------------------------------------------
void Map::BuildMesh(ShaderProgramRef shader)
{
	geomMap.Reset();

	for (size_t x = 0; x < AreaSizeXZ; x++)
	{
		for (size_t z = 0; z < AreaSizeXZ; z++)
		{
			if (area->tiles[x][z].type == TileType::None) continue;

			if (area->tiles[x][z].type == TileType::Solid)
				addSolid(shader, x, z);
			if (area->tiles[x][z].type == TileType::RampL)
				addRampL(shader, x, z);
			if (area->tiles[x][z].type == TileType::RampR)
				addRampR(shader, x, z);
			if (area->tiles[x][z].type == TileType::RampT)
				addRampT(shader, x, z);
			if (area->tiles[x][z].type == TileType::RampB)
				addRampB(shader, x, z);
		}
	}

	geomMap.Create(shader);
}
//-----------------------------------------------------------------------------
size_t Map::getIdArray(ShaderProgramRef shader, uint8_t textureId)
{
	size_t id = 99999;
	for (int i = 0; i < geomMap.arr.size(); i++)
	{
		if (geomMap.arr[i].textureId == textureId)
		{
			id = i;
			break;
		}
	}
	if (id == 99999)
	{
		id = geomMap.AddGeomBuff(shader);
		geomMap.arr[id].textureId = textureId;
	}
	return id;
}
//-----------------------------------------------------------------------------
void Map::addSolid(ShaderProgramRef shader, size_t x, size_t z)
{
	const QuadDim d(area, x, z);

	// вверх
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureTopId);
		
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z1 }, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z1 }, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z2 }, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z2 }, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}
	// лево
	if (x == 0 // слева есть тайл?
		|| (x > 0 && area->tiles[x - 1][z].type != TileType::Solid) // этот тайл сплошной?
		|| (x > 0 && area->tiles[x][z].posY != area->tiles[x - 1][z].posY)) // или между тайлами разница высоты?
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureWallId);

		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z1},         {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z2},         {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF1, d.z1}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f + d.razF1}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF1, d.z2}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f + d.razF1}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}

	// право
	if (x == AreaSizeXZ - 1  // справа есть тайл?
		|| (x < AreaSizeXZ && area->tiles[x + 1][z].type != TileType::Solid) // этот тайл сплошной?
		|| (x < AreaSizeXZ && area->tiles[x][z].posY != area->tiles[x + 1][z].posY)) // или между тайлами разница высоты?
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureWallId);

		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z2},         {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z1},         {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF2, d.z2}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f + d.razF2}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF2, d.z1}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f + d.razF2}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}

	// перед
	if (z == 0   // впереди есть тайл?
		|| (z > 0 && area->tiles[x][z - 1].type != TileType::Solid) // этот тайл сплошной?
		|| (z > 0 && area->tiles[x][z].posY != area->tiles[x][z - 1].posY)) // или между тайлами разница высоты?
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureWallId);

		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z2},         {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z2},         {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF3, d.z2}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f + d.razF3}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF3, d.z2}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f + d.razF3}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}

	// зад
	if (z == AreaSizeXZ - 1 // сзади есть тайл?
		|| (z < AreaSizeXZ && area->tiles[x][z + 1].type != TileType::Solid) // этот тайл сплошной?
		|| (z < AreaSizeXZ && area->tiles[x][z].posY != area->tiles[x][z + 1].posY)) // или между тайлами разница высоты?
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureWallId);

		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z1},         {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z1},         {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF4, d.z1}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f + d.razF4}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF4, d.z1}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f + d.razF4}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}
}
//-----------------------------------------------------------------------------
void Map::addRampL(ShaderProgramRef shader, size_t x, size_t z)
{
	const QuadDim d(area, x, z);

	// вверх
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureTopId);

		// TODO: увеличить размер текстуры так как скат по длине длинее боков
		// TODO: вместо razF1 нужно брать правильную версию
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF1, d.z1 }, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z1 }, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF1, d.z2 }, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z2 }, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}

	// право
	if (x == AreaSizeXZ - 1  // справа есть тайл?
		|| (x < AreaSizeXZ && area->tiles[x + 1][z].type != TileType::Solid) // этот тайл сплошной?
		|| (x < AreaSizeXZ && area->tiles[x][z].posY != area->tiles[x + 1][z].posY)) // или между тайлами разница высоты?
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureWallId);

		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z2},         {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z1},         {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF2, d.z2}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f + d.razF2}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF2, d.z1}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f + d.razF2}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}

	// перед
	if (z == 0   // впереди есть тайл?
		|| (z > 0 && area->tiles[x][z - 1].type != TileType::Solid) // этот тайл сплошной?
		|| (z > 0 && area->tiles[x][z].posY != area->tiles[x][z - 1].posY)) // или между тайлами разница высоты?
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureWallId);

		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z2},         {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF3, d.z2}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f + d.razF3}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF3, d.z2}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f + d.razF3}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertTriInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}

	// зад
	if (z == AreaSizeXZ - 1 // сзади есть тайл?
		|| (z < AreaSizeXZ && area->tiles[x][z + 1].type != TileType::Solid) // этот тайл сплошной?
		|| (z < AreaSizeXZ && area->tiles[x][z].posY != area->tiles[x][z + 1].posY)) // или между тайлами разница высоты?
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureWallId);

		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z1},         {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF4, d.z1}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f + d.razF4}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF4, d.z1}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f + d.razF4}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertTriInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}
}
//-----------------------------------------------------------------------------
void Map::addRampR(ShaderProgramRef shader, size_t x, size_t z)
{
	const QuadDim d(area, x, z);

	// вверх
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureTopId);

		// TODO: увеличить размер текстуры так как скат по длине длинее боков
		// TODO: вместо razF1 нужно брать правильную версию
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z1 }, {-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF1, d.z1 }, {-0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z2 }, {-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF1, d.z2 }, {-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}
	// лево
	if (x == 0 // слева есть тайл?
		|| (x > 0 && area->tiles[x - 1][z].type != TileType::Solid) // этот тайл сплошной?
		|| (x > 0 && area->tiles[x][z].posY != area->tiles[x - 1][z].posY)) // или между тайлами разница высоты?
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureWallId);

		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z1},         {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z2},         {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF1, d.z1}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f + d.razF1}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF1, d.z2}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f + d.razF1}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}

	// перед
	if (z == 0   // впереди есть тайл?
		|| (z > 0 && area->tiles[x][z - 1].type != TileType::Solid) // этот тайл сплошной?
		|| (z > 0 && area->tiles[x][z].posY != area->tiles[x][z - 1].posY)) // или между тайлами разница высоты?
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureWallId);

		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z2},         {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF3, d.z2}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f + d.razF3}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF3, d.z2}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f + d.razF3}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertTriInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}

	// зад
	if (z == AreaSizeXZ - 1 // сзади есть тайл?
		|| (z < AreaSizeXZ && area->tiles[x][z + 1].type != TileType::Solid) // этот тайл сплошной?
		|| (z < AreaSizeXZ && area->tiles[x][z].posY != area->tiles[x][z + 1].posY)) // или между тайлами разница высоты?
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureWallId);

		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z1},         {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF4, d.z1}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f + d.razF4}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF4, d.z1}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f + d.razF4}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertTriInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}
}
//-----------------------------------------------------------------------------
void Map::addRampT(ShaderProgramRef shader, size_t x, size_t z)
{
	const QuadDim d(area, x, z);

	// вверх
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureTopId);

		// TODO: увеличить размер текстуры так как скат по длине длинее боков
		// TODO: вместо razF1 нужно брать правильную версию
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z1 }, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z1 }, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF1, d.z2 }, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF1, d.z2 }, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}
	// лево
	if (x == 0 // слева есть тайл?
		|| (x > 0 && area->tiles[x - 1][z].type != TileType::Solid) // этот тайл сплошной?
		|| (x > 0 && area->tiles[x][z].posY != area->tiles[x - 1][z].posY)) // или между тайлами разница высоты?
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureWallId);

		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z1},         {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF1, d.z1}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f + d.razF1}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF1, d.z2}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f + d.razF1}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertTriInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}

	// право
	if (x == AreaSizeXZ - 1  // справа есть тайл?
		|| (x < AreaSizeXZ && area->tiles[x + 1][z].type != TileType::Solid) // этот тайл сплошной?
		|| (x < AreaSizeXZ && area->tiles[x][z].posY != area->tiles[x + 1][z].posY)) // или между тайлами разница высоты?
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureWallId);

		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z1},         {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF2, d.z2}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f + d.razF2}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF2, d.z1}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f + d.razF2}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertTriInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}

	// зад
	if (z == AreaSizeXZ - 1 // сзади есть тайл?
		|| (z < AreaSizeXZ && area->tiles[x][z + 1].type != TileType::Solid) // этот тайл сплошной?
		|| (z < AreaSizeXZ && area->tiles[x][z].posY != area->tiles[x][z + 1].posY)) // или между тайлами разница высоты?
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureWallId);

		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z1},         {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z1},         {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF4, d.z1}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f + d.razF4}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF4, d.z1}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f + d.razF4}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}
}
//-----------------------------------------------------------------------------
void Map::addRampB(ShaderProgramRef shader, size_t x, size_t z)
{
	const QuadDim d(area, x, z);

	// вверх
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureTopId);

		// TODO: увеличить размер текстуры так как скат по длине длинее боков
		// TODO: вместо razF1 нужно брать правильную версию
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z2 },         {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z2 },         {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF1, d.z1 }, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF1, d.z1 }, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}
	// лево
	if (x == 0 // слева есть тайл?
		|| (x > 0 && area->tiles[x - 1][z].type != TileType::Solid) // этот тайл сплошной?
		|| (x > 0 && area->tiles[x][z].posY != area->tiles[x - 1][z].posY)) // или между тайлами разница высоты?
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureWallId);

		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z2},         {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF1, d.z1}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f + d.razF1}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF1, d.z2}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f + d.razF1}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertTriInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}

	// право
	if (x == AreaSizeXZ - 1  // справа есть тайл?
		|| (x < AreaSizeXZ && area->tiles[x + 1][z].type != TileType::Solid) // этот тайл сплошной?
		|| (x < AreaSizeXZ && area->tiles[x][z].posY != area->tiles[x + 1][z].posY)) // или между тайлами разница высоты?
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureWallId);

		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z2},         {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF2, d.z2}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f + d.razF2}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF2, d.z1}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f + d.razF2}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertTriInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}

	// перед
	if (z == 0   // впереди есть тайл?
		|| (z > 0 && area->tiles[x][z - 1].type != TileType::Solid) // этот тайл сплошной?
		|| (z > 0 && area->tiles[x][z].posY != area->tiles[x][z - 1].posY)) // или между тайлами разница высоты?
	{
		const size_t id = getIdArray(shader, area->tiles[x][z].textureWallId);

		geomMap.arr[id].vertices.push_back({ { d.x1, d.y2, d.z2},         {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y2, d.z2},         {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x1, d.y1 - d.razF3, d.z2}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f + d.razF3}, {1.0f, 1.0f, 1.0f, 1.0f} });
		geomMap.arr[id].vertices.push_back({ { d.x2, d.y1 - d.razF3, d.z2}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f + d.razF3}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(geomMap.arr[id].indices, geomMap.arr[id].numIndex);
	}
}
//-----------------------------------------------------------------------------
void Map::insertQuadInIndexBuffer(std::vector<unsigned>& indices, unsigned& numIndex)
{
	indices.push_back(numIndex + 0);
	indices.push_back(numIndex + 1);
	indices.push_back(numIndex + 2);
	indices.push_back(numIndex + 2);
	indices.push_back(numIndex + 1);
	indices.push_back(numIndex + 3);
	numIndex += 4;
}
//-----------------------------------------------------------------------------
void Map::insertTriInIndexBuffer(std::vector<unsigned>& indices, unsigned& numIndex)
{
	indices.push_back(numIndex + 0);
	indices.push_back(numIndex + 1);
	indices.push_back(numIndex + 2);
	numIndex += 3;
}
//-----------------------------------------------------------------------------