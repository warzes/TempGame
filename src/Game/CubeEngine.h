#pragma once

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 96
#define CHUNK_SIZE_Z 16

constexpr unsigned AreaSizeXZ = 10000;

struct Tile3DDesc
{

};

struct Tile3DRef
{
	Tile3DDesc* ref = nullptr;
	int height = 0.0f;


};

struct Area
{
	Tile3DRef tiles[AreaSizeXZ][AreaSizeXZ];
};

constexpr float s = sizeof(Area)/1024.0/1024.0;