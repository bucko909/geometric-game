#include <math.h>
#include <iostream>
#include "euc.hpp"

#ifndef _EUC_C
#define _EUC_C

namespace bucko {

template <class T> EucVector3<T>::EucVector3(T x, T y, T z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

template <class T> EucVector3<T>::EucVector3(const EmbeddedVector<T> &_from) {
	EucVector3& from = (EucVector3&)_from;
	this->x = from.x;
	this->y = from.y;
	this->z = from.z;
}

// Cross product.
template <class T> EmbeddedVector<T>& EucVector3<T>::operator*=(const EmbeddedVector<T> &_with) {
	EucVector3<T>& with = (EucVector3<T>&)_with;
	T x = this->x;
	T y = this->y;
	this->x = y * with.z - this->z * with.y;
	this->y = this->z * with.x - x * with.z;
	this->z = x * with.y - y * with.x;
	return *this;
}

template <class T> EmbeddedVector<T>& EucVector3<T>::operator*(const EmbeddedVector<T> &with) const {
	EucVector3<T> ret = *this;
	ret *= with;
	return ret;
}

// Scale
template <class T> EmbeddedVector<T>& EucVector3<T>::operator*=(const T by) {
	x *= by;
	y *= by;
	z *= by;
	return *this;
}

template <class T> EmbeddedVector<T>& EucVector3<T>::operator/=(const T by) {
	x /= by;
	y /= by;
	z /= by;
	return *this;
}

template <class T> EmbeddedVector<T>& EucVector3<T>::operator*(const T rhs) const {
	EucVector3<T> ret = *this;
	ret *= rhs;
	return ret;
}

template <class T> EmbeddedVector<T>& EucVector3<T>::operator/(const T rhs) const {
	EucVector3<T> ret = *this;
	ret /= rhs;
	return ret;
}

// Scalar product
template <class T> T EucVector3<T>::operator&(const EmbeddedVector<T>& _with) const {
	EucVector3<T>& with = (EucVector3<T>&)_with;
	return x * with.x + y * with.y + z * with.z;
}

// Vector addition
template <class T> EmbeddedVector<T>& EucVector3<T>::operator+=(const EmbeddedVector<T> &_vec) {
	EucVector3<T>& vec = (EucVector3<T>&)_vec;
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

template <class T> EmbeddedVector<T>& EucVector3<T>::operator-=(const EmbeddedVector<T> &_vec) {
	EucVector3<T>& vec = (EucVector3<T>&)_vec;
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

template <class T> EmbeddedVector<T>& EucVector3<T>::operator+(const EmbeddedVector<T> &rhs) const {
	EucVector3<T> ret = *this;
	ret += rhs;
	return ret;
}

template <class T> EmbeddedVector<T>& EucVector3<T>::operator-(const EmbeddedVector<T> &rhs) const {
	EucVector3<T> ret = *this;
	ret -= rhs;
	return ret;
}

template <class T> EmbeddedVector<T>& EucVector3<T>::operator-() const {
	return EucVector3<T>(-x, -y, -z);
}

template <class T> EucVector3<T>::operator T() const {
	return sqrt(*this & *this);
}

template <class T> EmbeddedVector<T>& EucVector3<T>::operator~() const {
	EucVector3<T> ret = *this;
	ret /= (T)ret;
	return ret;
}

template <class T> void EucVector3<T>::normalise() {
	*this /= (T)(*this);
}

template <class T> EmbeddedVector<T>& EucVector3<T>::rotateClockwise(const EmbeddedVector<T> &_axis, const T angle) const {
	EucVector3<T>& axis = (EucVector3<T>&)_axis;
	T sine = sin(-angle);
	T cosine = cos(-angle);
	T cosine_comp = 1 - cosine;
	EucVector3<T> ret = EucVector3<T>(
		(cosine_comp * axis.x * axis.x + cosine) * x + (cosine_comp * axis.x * axis.y - sine * axis.z) * y + (cosine_comp * axis.x * axis.z + sine * axis.y) * z,
		(cosine_comp * axis.x * axis.y + sine * axis.z) * x + (cosine_comp * axis.y * axis.y + cosine) * y + (cosine_comp * axis.y * axis.z - sine * axis.x) * z,
		(cosine_comp * axis.y * axis.z - sine * axis.y) * x + (cosine_comp * axis.y * axis.z + sine * axis.x) * y + (cosine_comp * axis.z * axis.z + cosine) * z);
	return ret;
}

template <class T> EucPoint3<T>::EucPoint3(T x, T y, T z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

// Vector from a to b.
template <class T> EmbeddedVector<T>& EucPoint3<T>::operator-(const EmbeddedPoint<T> &_rhs) const {
	EucPoint3<T>& rhs = (EucPoint3<T>&)_rhs;
	return EucVector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
}

// Add a vector to a point
template <class T> EmbeddedPoint<T>& EucPoint3<T>::operator+=(const EmbeddedVector<T> &_vec) {
	EucVector3<T>& vec = (EucVector3<T>&)_vec;
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

template <class T> EmbeddedPoint<T>& EucPoint3<T>::operator-=(const EmbeddedVector<T> &_vec) {
	EucVector3<T>& vec = (EucVector3<T>&)_vec;
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

template <class T> EmbeddedPoint<T>& EucPoint3<T>::operator+(const EmbeddedVector<T> &rhs) const {
	EucPoint3<T> ret = *this;
	ret += rhs;
	return ret;
}

template <class T> EmbeddedPoint<T>& EucPoint3<T>::operator-(const EmbeddedVector<T> &rhs) const {
	EucPoint3<T> ret = *this;
	ret -= rhs;
	return ret;
}

template <class T> void EucVector3<T>::output(std::ostream& ostr) const {
	ostr << "(" << x << ", " << y << ", " << z << ")";
	return ostr;
}

template <class T> void EucPoint3<T>::output(std::ostream& ostr) const {
	ostr << "(" << x << ", " << y << ", " << z << ")";
	return ostr;
}

}
#endif //_EUC_C
