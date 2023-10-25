#pragma once

#include "SVGShape.h"
#include "LineSegment2D.h"

class SVGLine final : public SVGShape {
public:
	SVGLine(const LineSegment2D<float>&, SVGColor, float opacity = 1.0f);

	std::string getShapeTag() const override;
	Rectangle2D<float> computeRectangle() const override;

private:
	LineSegment2D<float> line;
};