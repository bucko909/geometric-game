#include <iostream>

#ifndef _POINT_H
#define _POINT_H

namespace bucko {

class Point3;

class Vector3 {
public:
	float x;
	float y;
	float z;

	Vector3(const Point3 &from, const Point3 &to);
	Vector3(float x, float y, float z);

	// Cross product
	Vector3& operator*=(const Vector3 &with);
	friend Vector3 operator*(const Vector3 &lhs, const Vector3 &rhs);

	// Scalar mult
	Vector3& operator*=(const float by);
	Vector3& operator/=(const float by);
	friend Vector3 operator*(const Vector3 &lhs, const float rhs);
	friend Vector3 operator/(const Vector3 &lhs, const float rhs);
	friend Vector3 operator*(const float lhs, const Vector3 &rhs);

	// Dot product
	friend float operator&(const Vector3 &lhs, const Vector3 &rhs);

	// Add and subtract
	Vector3& operator+=(const Vector3 &with);
	friend Vector3 operator+(const Vector3 &lhs, const Vector3 &rhs);
	Vector3& operator-=(const Vector3 &with);
	friend Vector3 operator-(const Vector3 &lhs, const Vector3 &rhs);

	// Can do that to points, too
	friend Point3 operator+(const Point3 &lhs, const Vector3 &rhs);
	friend Point3 operator+(const Vector3 &lhs, const Point3 &rhs);

	// Negate
	friend Vector3 operator-(const Vector3 &vec);

	// Length
	float length() const;

	// Normalise
	friend Vector3 operator~(const Vector3 &vec);
	friend std::ostream &operator<<(std::ostream &ostr, const Vector3 &vec);

	// Clockwise rotation when looking along axis
	Vector3 rotateClockwise(const Vector3 &axis, const float angle);
};

class Point3 {
public:
	float x;
	float y;
	float z;

	Point3(float x, float y, float z);

	// Direction vector
	friend Vector3 operator-(const Point3 &lhs, const Point3 &rhs);

	Vector3 directionTo(const Point3 &to) const;

	// Add a vector
	friend Point3 operator+(const Point3 &lhs, const Vector3 &rhs);
	friend Point3 operator-(const Point3 &lhs, const Vector3 &rhs);
	Point3& operator+=(const Vector3 &with);
	Point3& operator-=(const Vector3 &with);

	Point3 addVector(const Vector3 &vec) const;
	friend std::ostream &operator<<(std::ostream &ostr, const Point3 &vec);
};

}

#endif // _POINT_H
