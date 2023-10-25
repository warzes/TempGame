#pragma once

#include "Common.h"
#include "Earcut.h"
#include "NavTriangle.h"

namespace mapbox::util {

	template<> struct nth<0, Point2<float>> {
		static auto get(const Point2<float>& t);
	};

	template<> struct nth<1, Point2<float>> {
		static auto get(const Point2<float>& t);
	};
}

class TriangulationAlgorithm {
public:
	explicit TriangulationAlgorithm(std::vector<std::vector<Point2<float>>>&&);

	const std::vector<std::shared_ptr<NavTriangle>>& triangulate();

private:
	uint_fast64_t computeEdgeId(std::size_t, std::size_t) const;
	void linkTriangles(const std::map<uint_fast64_t, std::vector<std::shared_ptr<NavTriangle>>>&) const;

	std::vector<std::vector<Point2<float>>> polygonPoints;
	std::vector<std::shared_ptr<NavTriangle>> navTriangles;
};