#include "stdafx.h"
#include "ConvexHullModel.h"

ConvexHullModel::ConvexHullModel(ConvexHull3D<float> convexHull) :
	convexHull(std::move(convexHull)) {

}

std::vector<Point3<float>> ConvexHullModel::retrieveVertexArray(std::vector<uint32_t>&) const {
	std::vector<Point3<float>> vertexArray;
	vertexArray.reserve(convexHull.getIndexedTriangles().size() * 3);

	auto& convexHullPoints = convexHull.getConvexHullPoints();
	for (const auto& [triangleId, triangle] : convexHull.getIndexedTriangles()) {
		vertexArray.push_back(convexHullPoints.at(triangle.getIndex(0)).point);
		vertexArray.push_back(convexHullPoints.at(triangle.getIndex(2)).point);
		vertexArray.push_back(convexHullPoints.at(triangle.getIndex(1)).point);
	}

	return vertexArray;
}

PrimitiveTopology ConvexHullModel::getShapeType() const {
	return PrimitiveTopology::Triangles;
}