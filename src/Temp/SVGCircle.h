#pragma once

#include "SVGShape.h"

class SVGCircle final : public SVGShape {
public:
	SVGCircle(const Point2<float>&, float, SVGColor, float opacity = 1.0f);

	std::string getShapeTag() const override;
	Rectangle2D<float> computeRectangle() const override;

private:
	Point2<float> center;
	float radius;
};