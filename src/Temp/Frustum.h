#pragma once

#include "ConvexObject3D.h"
#include "AABBox.h"
#include "Plane.h"
#include "Matrix4.h"

/**
* Represents a frustum (truncated pyramid) in space
*/
template<class T> class nFrustum final : public ConvexObject3D<T> {
public:
	nFrustum();
	nFrustum(T, T, T, T);
	nFrustum(const Point3<T>&, const Point3<T>&, const Point3<T>&, const Point3<T>&, const Point3<T>&, const Point3<T>&, const Point3<T>&, const Point3<T>&);

	enum FrustumPoint {
		NTL = 0, //Near top left
		NTR, //Near top right
		NBL, //Near bottom left
		NBR, //Near bottom right
		FTL, //Far top left
		FTR, //Far top right
		FBL, //Far bottom left
		FBR //Far bottom right
	};

	void buildFrustum(T, T, T, T);
	void buildFrustum(const Point3<T>&, const Point3<T>&, const Point3<T>&, const Point3<T>&, const Point3<T>&, const Point3<T>&, const Point3<T>&, const Point3<T>&);

	const std::array<Point3<T>, 8>& getFrustumPoints() const;
	const Point3<T>& getFrustumPoint(FrustumPoint frustumPoint) const;
	const Point3<T>& getEyePosition() const;
	Point3<T> computeCenterPosition() const;

	Point3<T> getSupportPoint(const Vector3<T>&) const override;
	T computeNearDistance() const;
	T computeFarDistance() const;
	nFrustum<T> splitFrustum(T, T) const;
	nFrustum<T> cutFrustum(T) const;

	bool collideWithPoint(const Point3<T>&) const override;
	bool collideWithAABBox(const AABBox<T>&) const override;
	bool collideWithSphere(const nSphere<T>&) const override;
	void planesIntersectPoints(const Line3D<T>&, Point3<T>&, Point3<T>&, bool&) const;

private:
	void buildData();

	std::array<Point3<T>, 8> frustumPoints;
	Point3<T> eyePosition;
	std::array<nPlane<T>, 6> planes;
	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};
};

template<class T> nFrustum<T> operator *(const Matrix4<T>&, const nFrustum<T>&);
template<class T> nFrustum<T> operator *(const nFrustum<T>&, const Matrix4<T>&);

template<class T> std::ostream& operator <<(std::ostream&, const nFrustum<T>&);