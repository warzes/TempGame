#pragma once

#include "Common.h"
#include "NavMeshAgent.h"
#include "NavTopography.h"
#include "CSGPolygon.h"

enum class PolytopeType {
	PLANE,
	TERRAIN
};

class Polytope;

class PolytopeSurface {
public:
	explicit PolytopeSurface(PolytopeType);
	virtual ~PolytopeSurface() = default;

	PolytopeType getPolytopeType() const;

	void setPolytope(const Polytope*);
	const Polytope* getPolytope() const;
	std::size_t getSurfacePosition() const;

	void setWalkableCandidate(bool);
	bool isWalkableCandidate() const;

	virtual bool isWalkable() const = 0;
	virtual Rectangle2D<float> computeXZRectangle() const = 0;
	virtual const AABBox<float>& getAABBox() const = 0;

	virtual const std::vector<Point2<float>>& getOutlineCwPoints() const = 0;
	virtual nPlane<float> getPlane(const Rectangle2D<float>&) const = 0;
	virtual const std::vector<CSGPolygon<float>>& getSelfObstacles() const = 0;
	virtual Point3<float> computeRealPoint(const Point2<float>&, const NavMeshAgent&) const = 0;
	virtual const std::shared_ptr<const NavTopography>& getNavTopography() const = 0;

private:
	std::size_t computeSurfacePosition() const;

	PolytopeType polytopeType;
	const Polytope* polytope;
	std::size_t surfacePosition; //position/index of surface in polytope
	bool walkableCandidate;
};