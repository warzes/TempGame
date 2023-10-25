#include "stdafx.h"
#include "SVGShape.h"

SVGShape::SVGShape(SVGColor color, float opacity) :
	color(color),
	opacity(opacity),
	strokeColor(SVGShape::Black),
	strokeSize(0) {

}

void SVGShape::setStroke(SVGColor strokeColor, float strokeSize) {
	this->strokeColor = strokeColor;
	this->strokeSize = strokeSize;
}

std::string SVGShape::getStyle() const {
	return "fill:" + toColorString(color) + ";" +
		"stroke:" + toColorString(strokeColor) + ";stroke-width:" + std::to_string(strokeSize) + ";" +
		"opacity:" + std::to_string(opacity);
}

std::string SVGShape::toColorString(SVGColor color) const {
	switch (color) {
	case SVGColor::Black:
		return "black";
	case SVGColor::Lime:
		return "lime";
	case SVGColor::Red:
		return "red";
	case SVGColor::Blue:
		return "blue";
	case SVGColor::Orange:
		return "orange";
	case SVGColor::Green:
		return "green";
	case SVGColor::Yellow:
		return "yellow";
	default:
		throw std::runtime_error("Unknown SVG color: " + std::to_string(color));
	}
}