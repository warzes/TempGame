#pragma once

#include "SVGShape.h"
#include "Point2.h"

class SVGPolygon final : public SVGShape {
public:
	SVGPolygon(std::vector<Point2<float>>, SVGColor, float opacity = 1.0f);

	std::string getShapeTag() const override;
	Rectangle2D<float> computeRectangle() const override;

private:
	std::vector<Point2<float>> polygonPoints;
};