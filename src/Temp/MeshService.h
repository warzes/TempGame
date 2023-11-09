#pragma once

#include "Common.h"

class ConstMesh;
struct Bone;

class MeshService 
{
public:
	MeshService() = delete;

	static void ComputeVerticesNormalsTangents(const ConstMesh&, const std::vector<Bone>&, std::vector<Point3<float>>&, std::vector<Vector3<float>>&, std::vector<Vector3<float>>&);
	static void ComputeNormalsTangents(const ConstMesh&, const std::vector<Point3<float>>&, std::vector<Vector3<float>>&, std::vector<Vector3<float>>&);

private:
	static void computeVertices(const ConstMesh&, const std::vector<Bone>&, std::vector<Point3<float>>&);
};