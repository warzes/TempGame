#pragma once

#include "Common.h"
#include "EdgeLinkResult.h"

class EdgeLinkDetection {
public:
	explicit EdgeLinkDetection(float);

	EdgeLinkResult detectLink(const LineSegment3D<float>&, const LineSegment3D<float>&) const;

private:
	const float jumpMaxLength;
	const float edgeLinkMinLength;
	const float equalityDistanceThreshold;

	bool pointsAreEquals(const Point3<float>&, const Point3<float>&) const;
	bool isCollinearLines(const Line3D<float>&, const Line3D<float>&) const;
	bool hasCollinearEdgesLink(const LineSegment3D<float>&, const LineSegment3D<float>&, float&, float&) const;

	bool canJumpThatFar(const Point3<float>&, const Point3<float>&) const;
	bool isProperJumpDirection(const LineSegment3D<float>&, const LineSegment3D<float>&, const Point3<float>&, const Point3<float>&) const;

	bool isRangeTooSmall(float, float, const LineSegment3D<float>&) const;
};