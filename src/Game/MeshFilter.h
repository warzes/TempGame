#pragma once

#include "Mesh.h"
#include "Model.h"

class MeshFilter {
public:
	virtual ~MeshFilter() = default;

	virtual bool isAccepted(const nModel&) const = 0;
	virtual bool isAccepted(const nMesh&) const = 0;
};