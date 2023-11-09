#pragma once

#include "ConvexObject3D.h"
#include "AABBox.h"
#include "Sphere.h"
#include "Line3D.h"
#include "BoxShape.h"
#include "Matrix4.h"
#include "Point3.h"
#include "Vector3.h"
#include "Quaternion.h"

/**
* Represents an oriented bounding box in space
*/
template<class T> class OBBox final : public ConvexObject3D<T> {
public:
	OBBox();
	explicit OBBox(const Vector3<T>&, const Point3<T>&, const Quaternion<T>&);
	explicit OBBox(const AABBox<T>&);
	explicit OBBox(const nSphere<T>&);

	T getHalfSize(unsigned int) const;
	const Vector3<T>& getHalfSizes() const;
	const Point3<T>& getCenterOfMass() const;
	const Vector3<T>& getAxis(unsigned int) const;
	const Vector3<T>& getNormalizedAxis(unsigned int) const;
	const Quaternion<T>& getOrientation() const;
	std::vector<Point3<T>> getPoints() const;
	Point3<T> getPoint(unsigned int) const;
	Vector3<T> getVectorPoint(unsigned int) const;

	Point3<T> getSupportPoint(const Vector3<T>&) const override;
	AABBox<T> toAABBox() const;

	bool collideWithOBBox(const OBBox<T>&) const override;
	bool collideWithAABBox(const AABBox<T>&) const override;
	Point3<T> intersectPoint(const Line3D<T>&, bool&) const override;

private:
	template<class U> bool separatedAxisTheoremCollision(const U&) const;
	bool pointInsideSquare(const Point3<T>&, const std::array<Point3<T>, 4>&, const Vector3<T>&) const;

	Vector3<T> halfSizes;
	Point3<T> centerOfMass;
	Quaternion<T> orientation;

	std::array<Vector3<T>, 3> axis;
	std::array<Vector3<T>, 3> normalizedAxis;
};

template<class T> OBBox<T> operator *(const Matrix4<T>&, const OBBox<T>&);
template<class T> OBBox<T> operator *(const OBBox<T>&, const Matrix4<T>&);

template<class T> std::ostream& operator <<(std::ostream&, const OBBox<T>&);

template<class T> template<class U> bool OBBox<T>::separatedAxisTheoremCollision(const U& box) const {
	//separated axis theorem (see http://jkh.me/files/tutorials/Separating%20Axis%20Theorem%20for%20Oriented%20Bounding%20Boxes.pdf)
	Vector3<T> distCenterPoint = getCenterOfMass().vector(box.getCenterOfMass());
	std::array<std::array<T, 3>, 3> aDotB = { {
	{normalizedAxis[0].dotProduct(box.getNormalizedAxis(0)), normalizedAxis[0].dotProduct(box.getNormalizedAxis(1)), normalizedAxis[0].dotProduct(box.getNormalizedAxis(2))},
	{normalizedAxis[1].dotProduct(box.getNormalizedAxis(0)), normalizedAxis[1].dotProduct(box.getNormalizedAxis(1)), normalizedAxis[1].dotProduct(box.getNormalizedAxis(2))},
	{normalizedAxis[2].dotProduct(box.getNormalizedAxis(0)), normalizedAxis[2].dotProduct(box.getNormalizedAxis(1)), normalizedAxis[2].dotProduct(box.getNormalizedAxis(2))}
	} };

	constexpr auto EPSILON = (T)0.00001; //projectionAxis could be near to zero: need epsilon to avoid rounding error

	//case 1, 2, 3 (projectionAxis = axis[0] | axis[1] | axis[2])
	for (unsigned int i = 0; i < 3; i++) {
		if (std::abs(distCenterPoint.dotProduct(normalizedAxis[i]))
	> this->getHalfSize(i)
			+ std::abs(box.getHalfSize(0) * aDotB[i][0])
			+ std::abs(box.getHalfSize(1) * aDotB[i][1])
			+ std::abs(box.getHalfSize(2) * aDotB[i][2])) [[unlikely]] {
				return false;
			}
	}

	//case 4, 5, 6 (projectionAxis = bbox.getAxis(0) | bbox.getAxis(1) | bbox.getAxis(2))
	for (unsigned int i = 0; i < 3; i++) {
		if (std::abs(distCenterPoint.dotProduct(box.getNormalizedAxis(i)))
	> std::abs(this->getHalfSize(0) * aDotB[0][i])
			+ std::abs(this->getHalfSize(1) * aDotB[1][i])
			+ std::abs(this->getHalfSize(2) * aDotB[2][i])
			+ box.getHalfSize(i)) [[unlikely]] {
				return false;
			}
	}

	//case 7 (projectionAxis = axis[0].crossProduct(bbox.getAxis(0)))
	if (std::abs(distCenterPoint.dotProduct(normalizedAxis[0].crossProduct(box.getNormalizedAxis(0)))) - EPSILON
> std::abs(this->getHalfSize(1) * aDotB[2][0])
+ std::abs(this->getHalfSize(2) * aDotB[1][0])
+ std::abs(box.getHalfSize(1) * aDotB[0][2])
+ std::abs(box.getHalfSize(2) * aDotB[0][1])) [[unlikely]] {
			return false;
		}

		//case 8 (projectionAxis = axis[0].crossProduct(bbox.getAxis(1)))
			if (std::abs(distCenterPoint.dotProduct(normalizedAxis[0].crossProduct(box.getNormalizedAxis(1)))) - EPSILON
> std::abs(this->getHalfSize(1) * aDotB[2][1])
+ std::abs(this->getHalfSize(2) * aDotB[1][1])
+ std::abs(box.getHalfSize(0) * aDotB[0][2])
+ std::abs(box.getHalfSize(2) * aDotB[0][0])) [[unlikely]] {
					return false;
				}

				//case 9 (projectionAxis = axis[0].crossProduct(bbox.getAxis(2)))
					if (std::abs(distCenterPoint.dotProduct(normalizedAxis[0].crossProduct(box.getNormalizedAxis(2)))) - EPSILON
> std::abs(this->getHalfSize(1) * aDotB[2][2])
+ std::abs(this->getHalfSize(2) * aDotB[1][2])
+ std::abs(box.getHalfSize(0) * aDotB[0][1])
+ std::abs(box.getHalfSize(1) * aDotB[0][0])) [[unlikely]] {
							return false;
						}

						//case 10 (projectionAxis = axis[1].crossProduct(bbox.getAxis(0)))
							if (std::abs(distCenterPoint.dotProduct(normalizedAxis[1].crossProduct(box.getNormalizedAxis(0)))) - EPSILON
> std::abs(this->getHalfSize(0) * aDotB[2][0])
+ std::abs(this->getHalfSize(2) * aDotB[0][0])
+ std::abs(box.getHalfSize(1) * aDotB[1][2])
+ std::abs(box.getHalfSize(2) * aDotB[1][1])) [[unlikely]] {
									return false;
								}

								//case 11 (projectionAxis = axis[1].crossProduct(bbox.getAxis(1)))
									if (std::abs(distCenterPoint.dotProduct(normalizedAxis[1].crossProduct(box.getNormalizedAxis(1)))) - EPSILON
> std::abs(this->getHalfSize(0) * aDotB[2][1])
+ std::abs(this->getHalfSize(2) * aDotB[0][1])
+ std::abs(box.getHalfSize(0) * aDotB[1][2])
+ std::abs(box.getHalfSize(2) * aDotB[1][0])) [[unlikely]] {
											return false;
										}

										//case 12 (projectionAxis = axis[1].crossProduct(bbox.getAxis(2)))
											if (std::abs(distCenterPoint.dotProduct(normalizedAxis[1].crossProduct(box.getNormalizedAxis(2)))) - EPSILON
> std::abs(this->getHalfSize(0) * aDotB[2][2])
+ std::abs(this->getHalfSize(2) * aDotB[0][2])
+ std::abs(box.getHalfSize(0) * aDotB[1][1])
+ std::abs(box.getHalfSize(1) * aDotB[1][0])) [[unlikely]] {
													return false;
												}

												//case 13 (projectionAxis = axis[2].crossProduct(bbox.getAxis(0)))
													if (std::abs(distCenterPoint.dotProduct(normalizedAxis[2].crossProduct(box.getNormalizedAxis(0)))) - EPSILON
> std::abs(this->getHalfSize(0) * aDotB[1][0])
+ std::abs(this->getHalfSize(1) * aDotB[0][0])
+ std::abs(box.getHalfSize(1) * aDotB[2][2])
+ std::abs(box.getHalfSize(2) * aDotB[2][1])) [[unlikely]] {
															return false;
														}

														//case 14 (projectionAxis = axis[2].crossProduct(bbox.getAxis(1)))
															if (std::abs(distCenterPoint.dotProduct(normalizedAxis[2].crossProduct(box.getNormalizedAxis(1)))) - EPSILON
> std::abs(this->getHalfSize(0) * aDotB[1][1])
+ std::abs(this->getHalfSize(1) * aDotB[0][1])
+ std::abs(box.getHalfSize(0) * aDotB[2][2])
+ std::abs(box.getHalfSize(2) * aDotB[2][0])) [[unlikely]] {
																	return false;
																}

																//case 15 (projectionAxis = axis[2].crossProduct(bbox.getAxis(2)))
																	return std::abs(distCenterPoint.dotProduct(normalizedAxis[2].crossProduct(box.getNormalizedAxis(2)))) - EPSILON
																	<= std::abs(this->getHalfSize(0) * aDotB[1][2])
																	+ std::abs(this->getHalfSize(1) * aDotB[0][2])
																	+ std::abs(box.getHalfSize(0) * aDotB[2][1])
																	+ std::abs(box.getHalfSize(1) * aDotB[2][0]);
}
