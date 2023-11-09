#pragma once

#include "Point3.h"
#include "Vector4.h"

template<class T> class Point4 {
public:
	Point4() noexcept;
	Point4(T Xu, T Yu, T Zu, T Wu) noexcept;
	explicit Point4(const Point2<T>&, T Zu = 0, T Wu = 1) noexcept;
	explicit Point4(const Point3<T>&, T Wu = 1) noexcept;
	explicit Point4(const Vector3<T>&, T wu = 1) noexcept;
	explicit Point4(const Vector4<T>&);
	Point4(const Point4<T>&) = default;
	Point4<T>& operator=(const Point4<T>&) = default;

	void setValues(T, T, T, T);
	void setNull();

	T distance(const Point4<T>&) const;
	T squareDistance(const Point4<T>&) const;
	Vector4<T> vector(const Point4<T>&) const;
	Vector4<T> toVector() const;
	[[nodiscard]] Point4<T> translate(const Vector4<T>&) const;
	Point4<T> divideByW() const;
	Point3<T> toPoint3() const;

	Point4<T> operator +() const;
	Point4<T> operator -() const;
	Point4<T> operator +(const Point4<T>&) const;
	Point4<T> operator -(const Point4<T>&) const;
	const Point4<T>& operator +=(const Point4<T>& v);
	const Point4<T>& operator -=(const Point4<T>& v);
	const Point4<T>& operator *=(T t);
	const Point4<T>& operator /=(T t);
	const Point4<T>& operator +=(T t);
	const Point4<T>& operator -=(T t);

	bool operator ==(const Point4<T>&) const;
	std::partial_ordering operator <=>(const Point4<T>&) const;
	bool isEqual(const Point4<T>&, T) const;

	T& operator [](std::size_t i);
	const T& operator [](std::size_t i) const;

	explicit operator T* ();
	explicit operator const T* () const;

	template<class NEW_TYPE> Point4<NEW_TYPE> cast() const;

	T X;
	T Y;
	T Z;
	T W;
};

template<class T> Point4<T> operator *(const Point4<T>&, T t);
template<class T> Point4<T> operator *(T t, const Point4<T>&);
template<class T> Point4<T> operator /(const Point4<T>&, T t);
template<class T> Point4<T> operator +(const Point4<T>&, T t);
template<class T> Point4<T> operator -(const Point4<T>&, T t);

template<class T> Point4<T> operator *(const Matrix4<T>&, const Point4<T>&);
template<class T> Point4<T> operator *(const Point4<T>&, const Matrix4<T>&);

template<class T> std::ostream& operator <<(std::ostream&, const Point4<T>&);