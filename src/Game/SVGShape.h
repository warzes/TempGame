#pragma once

#include "Rectangle2D.h"

class SVGShape {
public:
	enum SVGColor {
		Black,
		Lime,
		Red,
		Blue,
		Orange,
		Green,
		Yellow
	};

	SVGShape(SVGColor, float);
	virtual ~SVGShape() = default;

	void setStroke(SVGColor, float);

	virtual std::string getShapeTag() const = 0;
	virtual Rectangle2D<float> computeRectangle() const = 0;

protected:
	std::string getStyle() const;
	std::string toColorString(SVGColor) const;

	SVGColor color;
	float opacity;
	SVGColor strokeColor;
	float strokeSize;
};