#pragma once

#include "Camera.h"

class CameraSpaceService {
public:
	explicit CameraSpaceService(const nCamera&);

	Point2<float> worldSpacePointToScreenSpace(const Point3<float>&) const;

	nRay<float> screenPointToRay(const Point2<float>&, float) const;
	Line3D<float> screenPointToLine(const Point2<float>&) const;

private:
	Vector3<float> screenPointToDirection(const Point2<float>&) const;

	const nCamera& camera;
};