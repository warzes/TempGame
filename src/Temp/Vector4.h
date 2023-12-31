#pragma once

#include "Vector3.h"
#include "Matrix4.h"

template<class T> class Vector4 {
public:
	Vector4() noexcept;
	Vector4(T Xu, T Yu, T Zu, T Wu) noexcept;
	explicit Vector4(const Vector3<T>&, T Wu = 1) noexcept;
	Vector4(const Vector4<T>&) = default;
	Vector4<T>& operator=(const Vector4<T>&) = default;

	void setValues(T, T, T, T);
	void setNull();

	[[nodiscard]] Vector4<T> normalize() const;
	T length() const;
	T squareLength() const;
	T dotProduct(const Vector4<T>&) const;
	Vector3<T> xyz() const;

	Vector4<T> operator +() const;
	Vector4<T> operator -() const;
	Vector4<T> operator +(const Vector4<T>&) const;
	Vector4<T> operator -(const Vector4<T>&) const;
	Vector4<T> operator *(const Vector4<T>&) const;
	Vector4<T> operator /(const Vector4<T>&) const;
	const Vector4<T>& operator +=(const Vector4<T>&);
	const Vector4<T>& operator -=(const Vector4<T>&);
	const Vector4<T>& operator *=(const Vector4<T>&);
	const Vector4<T>& operator /=(const Vector4<T>&);
	const Vector4<T>& operator *=(T t);
	const Vector4<T>& operator /=(T t);

	bool operator ==(const Vector4<T>&) const;
	std::partial_ordering operator <=>(const Vector4<T>&) const;
	bool isEqual(const Vector4<T>&, T) const;

	T& operator [](std::size_t i);
	const T& operator [](std::size_t i) const;

	explicit operator T* ();
	explicit operator const T* () const;

	template<class NEW_TYPE> Vector4<NEW_TYPE> cast() const;

	T X;
	T Y;
	T Z;
	T W;
};

template<class T> Vector4<T> operator *(const Vector4<T>&, T t);
template<class T> Vector4<T> operator *(T t, const Vector4<T>&);
template<class T> Vector4<T> operator /(const Vector4<T>&, T t);

template<class T> Vector4<T> operator *(const Matrix4<T>&, const Vector4<T>&);
template<class T> Vector4<T> operator *(const Vector4<T>&, const Matrix4<T>&);

template<class T> std::ostream& operator <<(std::ostream&, const Vector4<T>&);