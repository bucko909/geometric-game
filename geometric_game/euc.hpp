#include <iostream>
#include "embedded.hpp"

#ifndef _EUC_H
#define _EUC_H

namespace bucko {

template <class T> class EucPoint3;

template <class T> class EucVector3 : public EmbeddedVector<T> {
public:
	T x;
	T y;
	T z;

	EucVector3<T>(const EmbeddedVector<T> &from);
	EucVector3<T>(T x, T y, T z);

	// Cross product
	virtual EmbeddedVector<T>& operator*=(const EmbeddedVector<T> &with);
	virtual EmbeddedVector<T>& operator*(const EmbeddedVector<T> &rhs) const;

	// Scalar mult
	virtual EmbeddedVector<T>& operator*=(const T by);
	virtual EmbeddedVector<T>& operator/=(const T by);
	virtual EmbeddedVector<T>& operator*(const T rhs) const;
	virtual EmbeddedVector<T>& operator/(const T rhs) const;

	// Dot product
	virtual T operator&(const EmbeddedVector<T> &rhs) const;

	// Add and subtract
	virtual EmbeddedVector<T>& operator+=(const EmbeddedVector<T> &with);
	virtual EmbeddedVector<T>& operator+(const EmbeddedVector<T> &rhs) const;
	virtual EmbeddedVector<T>& operator-=(const EmbeddedVector<T> &with);
	virtual EmbeddedVector<T>& operator-(const EmbeddedVector<T> &rhs) const;

	// Negate
	virtual EmbeddedVector<T>& operator-() const;

	// Length
	operator T() const;

	// Normalise
	virtual EmbeddedVector<T>& operator~() const;
	void normalise();

	virtual void output(std::ostream &ostr) const;

	// Clockwise rotation when looking along axis
	virtual EmbeddedVector<T>& rotateClockwise(const EmbeddedVector<T> &axis, const T angle) const;
};

template <class T> class EucPoint3 : public EmbeddedPoint<T> {
public:
	T x;
	T y;
	T z;

	EucPoint3<T>(T x, T y, T z);
	EucPoint3<T>(EmbeddedPoint<T> &from);

	// Direction vector
	virtual EmbeddedVector<T>& operator-(const EmbeddedPoint<T> &rhs) const;

	// Add a vector
	virtual EmbeddedPoint<T>& operator+(const EmbeddedVector<T> &rhs) const;
	virtual EmbeddedPoint<T>& operator-(const EmbeddedVector<T> &rhs) const;
	virtual EmbeddedPoint<T>& operator+=(const EmbeddedVector<T> &with);
	virtual EmbeddedPoint<T>& operator-=(const EmbeddedVector<T> &with);

	virtual void output(std::ostream &ostr) const;
};

}
#include "euc.cpp"

#endif // _EUC_H
