#include "stdafx.h"
#include "GeometryMap.h"
//-----------------------------------------------------------------------------
void GeometryMap::Create(ShaderProgramRef shader)
{
	auto& renderSystem = GetRenderSystem();

	for (size_t i = 0; i < arr.size(); i++)
	{
		if (arr[i].vertices.size() > 0)
		{
			renderSystem.UpdateBuffer(arr[i].geom->GetVBO(), 0, arr[i].vertices.size(), sizeof(TileVertex), arr[i].vertices.data());
			renderSystem.UpdateBuffer(arr[i].geom->GetIBO(), 0, arr[i].indices.size(), IndexFormat::UInt32, arr[i].indices.data());
		}
	}
}
//-----------------------------------------------------------------------------
size_t GeometryMap::AddGeomBuff(ShaderProgramRef shader)
{
	auto& renderSystem = GetRenderSystem();
	arr.push_back({ .geom = renderSystem.CreateGeometryBuffer(renderSystem.CreateVertexBuffer(BufferUsage::DynamicDraw), renderSystem.CreateIndexBuffer(BufferUsage::DynamicDraw), shader) });
	return arr.size() - 1;
}
//-----------------------------------------------------------------------------