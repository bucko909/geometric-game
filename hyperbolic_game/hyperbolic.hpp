#include <math.h>
#include "point.hpp"

#ifndef _HYPERBOLIC_H
#define _HYPERBOLIC_H

namespace bucko {

class HyperbolicPlane3;

class HyperbolicPoint3 : public Point3 {
private:
public:
	Vector3 directionTo(const HyperbolicPoint3 &to) const;
	Vector3 directionTo(const HyperbolicPlane3 &to) const;
	HyperbolicPoint3 addVector(Vector3 &vec, const float distance) const;
	HyperbolicPoint3 addVector(Vector3 &vec) const;
	Point3 disEmbed(const HyperbolicPoint3 &relativeTo) const;

	friend void glVertex3f(const HyperbolicPoint3 &eye, const HyperbolicPoint3 &target);

	friend void glVertex3f(const HyperbolicPoint3 &eye, const float x, const float y, const float z);

	friend void gluLookAt(const HyperbolicPoint3 &eye, const Vector3 &forward, const Vector3 &up);

	friend void gluLookAt(const HyperbolicPoint3 &eye, const HyperbolicPoint3 &target, const Vector3 &up);
	friend std::ostream &operator<<(std::ostream &ostr, const Point3 &vec);


	HyperbolicPoint3(float, float, float);

};

class HyperbolicPlane3 {
public:
	HyperbolicPoint3 centre;
	float radius;
	HyperbolicPlane3(const HyperbolicPoint3 &point1, const HyperbolicPoint3 &point2, const HyperbolicPoint3 &point3);
	HyperbolicPlane3(const HyperbolicPoint3 &centre, const float radius);
};

}

#endif // _HYPERBOLIC_H
