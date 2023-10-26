#pragma once

#include "Vertex.h"

constexpr unsigned AreaSizeXZ = 20;

enum class TileType
{
	None, // не рисуется
	Solid
	// рампы разных высот
	// лестницы
};

struct Tile3DDesc
{
	TileType type = TileType::None;

	inline bool operator==(const Tile3DDesc& ptr)
	{
		return (type == ptr.type);
	}

	inline bool operator!=(const Tile3DDesc& ptr)
	{
		return (type != ptr.type);
	}
};

struct Tile3DRef
{
	//Tile3DDesc* ref = nullptr; // в будущем сделать справочник тайлов и хранить только указатель, так как многие тайлы идентичны
	Tile3DDesc desc;
			
	/* -4 = -1.0
		0 - линия моря
		1 = 0.25
		2 = 0.5
		3 = 0.75
		4 = 1.0*/
	int8_t posY;
};

struct Area
{
	Tile3DRef tiles[AreaSizeXZ][AreaSizeXZ];
};

constexpr float s = sizeof(Area)/1024.0/1024.0;

struct Map
{
	Map()
	{
		area = new Area();
	}
	~Map()
	{
		delete area;
	}

	void SetTile(const Tile3DRef& tile, unsigned x, unsigned y);

	void BuildMesh(ShaderProgramRef shader);

	Area* area;
	GeometryBufferRef geom;

private:
	void addSolid(size_t x, size_t z, std::vector<BaseVertex>& vertices, std::vector<unsigned>& indices, unsigned& numIndex);
	void insertQuadInIndexBuffer(std::vector<unsigned>& indices, unsigned& numIndex);

};