#include <math.h>
#include <iostream>
#include "point.hpp"

namespace bucko {

Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3& Vector3::operator*=(const Vector3 &with) {
	float x = this->x;
	float y = this->y;
	this->x = y * with.z - this->z * with.y;
	this->y = this->z * with.x - x * with.z;
	this->z = x * with.y - y * with.x;
	return *this;
}

Vector3 operator*(const Vector3 &lhs, const Vector3 &with) {
	Vector3 ret = lhs;
	ret *= with;
	return ret;
}

Vector3& Vector3::operator*=(const float by) {
	x *= by;
	y *= by;
	z *= by;
	return *this;
}

Vector3& Vector3::operator/=(const float by) {
	x /= by;
	y /= by;
	z /= by;
	return *this;
}

Vector3 operator*(const Vector3 &lhs, const float rhs) {
	Vector3 ret = lhs;
	ret *= rhs;
	return ret;
}

Vector3 operator*(const float lhs, const Vector3 &rhs) {
	Vector3 ret = rhs;
	ret *= lhs;
	return ret;
}

Vector3 operator/(const Vector3 &lhs, const float rhs) {
	Vector3 ret = lhs;
	ret /= rhs;
	return ret;
}

float operator&(const Vector3 &first, const Vector3& second) {
	return first.x * second.x + first.y * second.y + first.z * second.z;
}

Vector3& Vector3::operator+=(const Vector3 &vec) {
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3 &vec) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

Vector3 operator+(const Vector3 &lhs, const Vector3 &rhs) {
	Vector3 ret = lhs;
	ret += rhs;
	return ret;
}

Vector3 operator-(const Vector3 &lhs, const Vector3 &rhs) {
	Vector3 ret = lhs;
	ret -= rhs;
	return ret;
}

Vector3 operator-(const Vector3 &vec) {
	return Vector3(-vec.x, -vec.y, -vec.z);
}

float Vector3::length() const {
	return sqrt(*this & *this);
}

Vector3 operator~(const Vector3 &vec) {
	Vector3 ret = vec;
	ret /= ret.length();
	return ret;
}

Vector3 Vector3::rotateClockwise(const Vector3 &axis, const float angle) {
	float sine = sin(-angle);
	float cosine = cos(-angle);
	float cosine_comp = 1 - cosine;
	Vector3 ret = Vector3(
		(cosine_comp * axis.x * axis.x + cosine) * x + (cosine_comp * axis.x * axis.y - sine * axis.z) * y + (cosine_comp * axis.x * axis.z + sine * axis.y) * z,
		(cosine_comp * axis.x * axis.y + sine * axis.z) * x + (cosine_comp * axis.y * axis.y + cosine) * y + (cosine_comp * axis.y * axis.z - sine * axis.x) * z,
		(cosine_comp * axis.y * axis.z - sine * axis.y) * x + (cosine_comp * axis.y * axis.z + sine * axis.x) * y + (cosine_comp * axis.z * axis.z + cosine) * z);
	return ret;
}

Point3::Point3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3 operator-(const Point3 &lhs, const Point3 &rhs) {
	return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

Vector3 Point3::directionTo(const Point3 &to) const {
	return to - *this;
}

Point3 &Point3::operator+=(const Vector3 &vec) {
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

Point3 &Point3::operator-=(const Vector3 &vec) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

Point3 operator+(const Point3 &lhs, const Vector3 &rhs) {
	Point3 ret = lhs;
	ret += rhs;
	return ret;
}

Point3 operator-(const Point3 &lhs, const Vector3 &rhs) {
	Point3 ret = lhs;
	ret -= rhs;
	return ret;
}

Point3 Point3::addVector(const Vector3 &dir) const {
	return *this + dir;
}

std::ostream &operator<<(std::ostream& ostr, const Point3& point) {
  ostr << "(" << point.x << ", " << point.y << ", " << point.z << ")";
  return ostr;
}

std::ostream &operator<<(std::ostream& ostr, const Vector3& point) {
  ostr << "(" << point.x << ", " << point.y << ", " << point.z << ")";
  return ostr;
}
}
