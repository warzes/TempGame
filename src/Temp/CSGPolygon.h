#pragma once

#include "Common.h"

template<class T> class CSGPolygon {
public:
	CSGPolygon(std::string name, const std::vector<Point2<T>>&);
	CSGPolygon(std::string name, std::vector<Point2<T>>&&);

	const std::string& getName() const;
	const std::vector<Point2<T>>& getCwPoints() const;

	T computeArea() const;
	bool pointInsidePolygon(const Point2<T>&) const;
	bool pointInsideOrOnPolygon(const Point2<T>&) const;

	void expand(T);
	void simplify(T, T);

private:
	bool pointInsidePolygon(const Point2<T>&, bool) const;

	bool isCwPoints() const;
	void logInputData(std::string_view, const CSGPolygon<T>&) const;

	std::string name;
	mutable std::vector<Point2<T>> cwPoints;

};

template<class T> std::ostream& operator <<(std::ostream&, const CSGPolygon<T>&);