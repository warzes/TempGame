#pragma once

#include "GeometryMap.h"

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

	void SetTile(const Tile3D& tile, unsigned x, unsigned y);

	void BuildMesh(ShaderProgramRef shader);

	Area* area;
	GeometryMap geomMap;

private:
	size_t getIdArray(ShaderProgramRef shader, uint8_t textureId);
	void addSolid(ShaderProgramRef shader, size_t x, size_t z);
	void addRampL(ShaderProgramRef shader, size_t x, size_t z);
	void addRampR(ShaderProgramRef shader, size_t x, size_t z);
	void addRampT(ShaderProgramRef shader, size_t x, size_t z);
	void addRampB(ShaderProgramRef shader, size_t x, size_t z);

	void insertQuadInIndexBuffer(std::vector<unsigned>& indices, unsigned& numIndex);
	void insertTriInIndexBuffer(std::vector<unsigned>& indices, unsigned& numIndex);
};