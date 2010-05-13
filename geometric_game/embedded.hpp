#ifndef _EMBEDDED_H
#define _EMBEDDED_H
namespace bucko {

// T should be double, float, ...
template <class T> class EmbeddedPoint;
template <class T> class EmbeddedVector {
public:
	// Cross product
	virtual EmbeddedVector<T>& operator*=(const EmbeddedVector<T> &with) = 0;
	virtual EmbeddedVector<T>& operator*(const EmbeddedVector<T> &rhs) const = 0;

	// Scalar mult
	virtual EmbeddedVector<T>& operator*=(const T by) = 0;
	virtual EmbeddedVector<T>& operator/=(const T by) = 0;
	virtual EmbeddedVector<T>& operator*(const T rhs) const = 0;
	virtual EmbeddedVector<T>& operator/(const T rhs) const = 0;

	// Dot product
	virtual T operator&(const EmbeddedVector<T> &rhs) const = 0;

	// Add and subtract
	virtual EmbeddedVector<T>& operator+=(const EmbeddedVector<T> &with) = 0;
	virtual EmbeddedVector<T>& operator+(const EmbeddedVector<T> &rhs) const = 0;
	virtual EmbeddedVector<T>& operator-=(const EmbeddedVector<T> &with) = 0;
	virtual EmbeddedVector<T>& operator-(const EmbeddedVector<T> &rhs) const = 0;

	// Negate
	virtual EmbeddedVector<T>& operator-() const = 0;

	// Length
	virtual operator T() const = 0;

	// Normalise
	virtual EmbeddedVector<T>& operator~() const = 0;
	virtual void normalise() = 0;

	virtual void output(std::ostream &ostr) const = 0;
	template <class U> friend std::ostream &operator<<(std::ostream &ostr, const EmbeddedVector<U> &vec);

	// Clockwise rotation when looking along axis
	virtual EmbeddedVector<T>& rotateClockwise(const EmbeddedVector<T> &axis, const T angle) const = 0;
};

template <class T> class EmbeddedPoint {
public:
	// Direction vector
	virtual EmbeddedVector<T>& operator-(const EmbeddedPoint<T> &rhs) const = 0;

	// Add a vector
	virtual EmbeddedPoint<T>& operator+(const EmbeddedVector<T> &rhs) const = 0;
	virtual EmbeddedPoint<T>& operator-(const EmbeddedVector<T> &rhs) const = 0;
	virtual EmbeddedPoint<T>& operator+=(const EmbeddedVector<T> &with) = 0;
	virtual EmbeddedPoint<T>& operator-=(const EmbeddedVector<T> &with) = 0;

	virtual void output(std::ostream &ostr) const = 0;
	template <class U> friend std::ostream &operator<<(std::ostream &ostr, const EmbeddedPoint<U> &vec);
};
}
#endif // _EMBEDDED_H
