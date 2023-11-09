#include "stdafx.h"
#include "RectangleShape.h"

template<class T> RectangleShape<T>::RectangleShape(const Vector2<T>& diagonal) :
	diagonal(diagonal) {

}

template<class T> const Vector2<T>& RectangleShape<T>::getDiagonal() const {
	return diagonal;
}

//explicit template
template class RectangleShape<int>;
template class RectangleShape<float>;