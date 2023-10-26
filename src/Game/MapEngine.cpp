#include "stdafx.h"
#include "MapEngine.h"
//-----------------------------------------------------------------------------
void Map::SetTile(const Tile3DRef& tile, unsigned x, unsigned y)
{
	area->tiles[x][y].desc = tile.desc;
	area->tiles[x][y].posY = tile.posY;
}
//-----------------------------------------------------------------------------
void Map::BuildMesh(ShaderProgramRef shader)
{
	geom.reset();

	std::vector<BaseVertex> vertices;
	std::vector<unsigned> indices;
	unsigned numIndex = 0;
	for (size_t x = 0; x < AreaSizeXZ; x++)
	{
		for (size_t z = 0; z < AreaSizeXZ; z++)
		{
			if (area->tiles[x][z].desc.type == TileType::None) continue;

			if (area->tiles[x][z].desc.type == TileType::Solid)
			{
				addSolid(x, z, vertices, indices, numIndex);
			}
		}
	}

	if (vertices.size() == 0) return;

	auto& renderSystem = GetRenderSystem();
	VertexBufferRef vb = renderSystem.CreateVertexBuffer(BufferUsage::DynamicDraw);
	IndexBufferRef ib = renderSystem.CreateIndexBuffer(BufferUsage::DynamicDraw);
	geom = renderSystem.CreateGeometryBuffer(vb, ib, shader);

	renderSystem.UpdateBuffer(geom->GetVBO(), 0, vertices.size(), sizeof(BaseVertex), vertices.data());
	renderSystem.UpdateBuffer(geom->GetIBO(), 0, indices.size(), IndexFormat::UInt32, indices.data());
}
//-----------------------------------------------------------------------------
void Map::addSolid(size_t x, size_t z, std::vector<BaseVertex>& vertices, std::vector<unsigned>& indices, unsigned& numIndex)
{
	const float x1 = -0.5f + x;
	const float x2 =  0.5f + x;
	const float y1 = -0.5f + area->tiles[x][z].posY / 4.0f;
	const float y2 =  0.5f + area->tiles[x][z].posY / 4.0f;
	const float z1 =  0.5f + z;
	const float z2 = -0.5f + z;

	int8_t raz1 = 0;
	if (x > 0) raz1 = area->tiles[x][z].posY - area->tiles[x - 1][z].posY;
	const float razF1 = (raz1 > 0) ? (raz1 / 4.0f - 1.0f) : 0.0f;

	int8_t raz2 = 0;
	if (x < AreaSizeXZ - 1) raz2 = area->tiles[x][z].posY - area->tiles[x + 1][z].posY;
	const float razF2 = (raz2 > 0) ? (raz2 / 4.0f - 1.0f) : 0.0f;

	int8_t raz3 = 0;
	if (z > 0) raz3 = area->tiles[x][z].posY - area->tiles[x][z - 1].posY;
	const float razF3 = (raz3 > 0) ? (raz3 / 4.0f - 1.0f) : 0.0f;

	int8_t raz4 = 0;
	if (z < AreaSizeXZ - 1) raz4 = area->tiles[x][z].posY - area->tiles[x][z + 1].posY;
	const float razF4 = (raz4 > 0) ? (raz4 / 4.0f - 1.0f) : 0.0f;

	// вверх
	{
		vertices.push_back({ { x1, y2, z1 }, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { x2, y2, z1 }, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { x1, y2, z2 }, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { x2, y2, z2 }, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(indices, numIndex);
	}
	// лево
	if (x == 0 // слева есть тайл?
		|| (x > 0 && area->tiles[x - 1][z].desc.type != TileType::Solid) // этот тайл сплошной?
		|| (x > 0 && area->tiles[x][z].posY != area->tiles[x - 1][z].posY)) // или между тайлами разница высоты?
	{
		vertices.push_back({ { x1, y2, z1},         {0.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { x1, y2, z2},         {1.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { x1, y1 - razF1, z1}, {0.0f, 1.0f + razF1}, {1.0f, 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { x1, y1 - razF1, z2}, {1.0f, 1.0f + razF1}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(indices, numIndex);
	}

	// право
	if (x == AreaSizeXZ - 1  // справа есть тайл?
		|| (x < AreaSizeXZ && area->tiles[x + 1][z].desc.type != TileType::Solid) // этот тайл сплошной?
		|| (x < AreaSizeXZ && area->tiles[x][z].posY != area->tiles[x + 1][z].posY)) // или между тайлами разница высоты?
	{
		vertices.push_back({ { x2, y2, z2},         {0.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { x2, y2, z1},         {1.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { x2, y1 - razF2, z2}, {0.0f, 1.0f + razF2}, {1.0f, 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { x2, y1 - razF2, z1}, {1.0f, 1.0f + razF2}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(indices, numIndex);
	}

	//// низ
	// наверное не нужно - нельзя увидеть дно
	//{
	//	vertices.push_back({
	//		{  0.5f + x, -0.5f + area->tiles[x][z].posY / 4.0f,  0.5f + z},
	//		{0.0f, 0.0f},
	//		{1.0f, 1.0f, 1.0f, 1.0f} });
	//	vertices.push_back({
	//		{ -0.5f + x, -0.5f + area->tiles[x][z].posY / 4.0f,  0.5f + z},
	//		{1.0f, 0.0f},
	//		{1.0f, 1.0f, 1.0f, 1.0f} });
	//	vertices.push_back({
	//		{  0.5f + x, -0.5f + area->tiles[x][z].posY / 4.0f, -0.5f + z},
	//		{0.0f, 1.0f},
	//		{1.0f, 1.0f, 1.0f, 1.0f} });
	//	vertices.push_back({
	//		{ -0.5f + x, -0.5f + area->tiles[x][z].posY / 4.0f, -0.5f + z},
	//		{1.0f, 1.0f},
	//		{1.0f, 1.0f, 1.0f, 1.0f} });
	//	insertQuadInIndexBuffer(indices, numIndex);
	//}

	// перед
	if (z == 0   // впереди есть тайл?
		|| (z > 0 && area->tiles[x][z - 1].desc.type != TileType::Solid) // этот тайл сплошной?
		|| (z > 0 && area->tiles[x][z].posY != area->tiles[x][z - 1].posY)) // или между тайлами разница высоты?
	{
		vertices.push_back({ { x1, y2, z2},         {0.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { x2, y2, z2},         {1.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { x1, y1 - razF3, z2}, {0.0f, 1.0f + razF3}, {1.0f, 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { x2, y1 - razF3, z2}, {1.0f, 1.0f + razF3}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(indices, numIndex);
	}

	// зад
	if (z == AreaSizeXZ - 1 // сзади есть тайл?
		|| (z < AreaSizeXZ && area->tiles[x][z + 1].desc.type != TileType::Solid) // этот тайл сплошной?
		|| (z < AreaSizeXZ && area->tiles[x][z].posY != area->tiles[x][z + 1].posY)) // или между тайлами разница высоты?
	{
		vertices.push_back({ { x2, y2, z1},         {0.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { x1, y2, z1},         {1.0f, 0.0f},         {1.0f, 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { x2, y1 - razF4, z1}, {0.0f, 1.0f + razF4}, {1.0f, 1.0f, 1.0f, 1.0f} });
		vertices.push_back({ { x1, y1 - razF4, z1}, {1.0f, 1.0f + razF4}, {1.0f, 1.0f, 1.0f, 1.0f} });

		insertQuadInIndexBuffer(indices, numIndex);
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