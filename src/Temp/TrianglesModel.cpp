#include "stdafx.h"
#include "TrianglesModel.h"

TrianglesModel::TrianglesModel(std::vector<Point3<float>> trianglesPoints) :
	trianglesPoints(std::move(trianglesPoints)) {
	if (this->trianglesPoints.size() % 3 != 0) {
		throw std::invalid_argument("Triangles points size must a multiple of three. Size: " + std::to_string(this->trianglesPoints.size()));
	}
}

std::vector<Point3<float>> TrianglesModel::retrieveVertexArray(std::vector<uint32_t>&) const {
	return trianglesPoints;
}

PrimitiveTopology TrianglesModel::getShapeType() const {
	return PrimitiveTopology::Triangles;
}