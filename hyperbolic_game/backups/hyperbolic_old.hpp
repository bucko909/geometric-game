#include <math.h>
#include "point.hpp"

#ifndef _HYPERBOLIC_H
#define _HYPERBOLIC_H

namespace bucko {

Point3 embeddingToLocalPoint_hyperbolic(const Point3 &from, const Point3 &to);
Point3 embeddingAddVector_hyperbolic(const Point3 &source, Vector3 &vec, const float distance);
Point3 embeddingAddVector_hyperbolic(const Point3 &source, Vector3 &vec);

void glVertex3f_hyperbolic(Point3 eye, Point3 target);

void glVertex3f_hyperbolic(Point3 eye, float x, float y, float z);

void gluLookAt_hyperbolic(Point3 eye, Vector3 forward, Vector3 up);

void gluLookAt_hyperbolic(Point3 eye, Point3 target, Vector3 up);

}

#endif // _HYPERBOLIC_H
