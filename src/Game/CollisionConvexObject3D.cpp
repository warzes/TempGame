#include "stdafx.h"
#include "CollisionConvexObject3D.h"

/**
* @param outerMargin Collision outer margin. Margin must be positive and will be added to the object to obtain his real size.
*/
CollisionConvexObject3D::CollisionConvexObject3D(float outerMargin) :
	outerMargin(outerMargin) {

}

float CollisionConvexObject3D::getOuterMargin() const {
	return outerMargin;
}