#include <math.h>
#include <stdio.h>
#include <GL/glfw.h>
#include "point.hpp"
#include "hyperbolic.hpp"

namespace bucko {

const float neginf = log(0);
const float inf = -neginf;
// Returns (inf, inf, 0) or (x, y, -inf)
HyperbolicPoint3 static HyperbolicPoint3::helperFindCircle(const HyperbolicPoint3 &point1, const HyperbolicPoint3 &point2) {
}

EuclideanPoint3 HyperbolicPoint3::disEmbed(const HyperbolicPoint3 &relativeTo) {
	// First, find the Euclidean direction vector
	Vector3 dir = *this - relativeTo;

	// And the midpoint of the line segment connecting the two points.
	Point3 mid = relativeTo + dir / 2.0f;

	// If we cross product the direction with (0,0,1), we get a vector
	// which is perpendicular to the direction and has z component equal to
	// zero - in particular, it will be parallel to the z=0 plane.
	Vector3 planar_norm = Vector3(0,0,1);
	Vector3 norm_1 = dir * planar_norm;

	// Cross producting this result with the direction vector yields a new
	// vector which must be a perpendicular bisector of our connecting line,
	// and of the bisector parallel to the z=0 plane. Thus, it must point at
	// the centre of the circle which is coplanar to eye and point, and is
	// perpendicular to the z=0 plane.
	Vector3 norm = dir * norm_1;

	// Here we special case. If the radius is (likely to be) huge compared to
	// our z position, we can safely assume that the circle is just a
	// vertical line.
	if (fabs(norm.z) > 0.0001 * mid.z) {
		// Now calculate the relative distance of the midpoint to z=0 plane
		// along this normal. The line intersects the centre of our circle at 
		// z=0, so this is the number of copies of norm we need to subtract
		// from mid to get to centre.
		float norm_mult = mid.z / norm.z;

		// Now we've got the centre everything else just drops out.
		Point3 centre = mid - norm * norm_mult;

		// The eye is on the circumference of the circle, hence is at distance
		// radius from the centre.
		float radius = (float)(centre - from);

		// Now we want the direction vector that the geodesic we found has at
		// the source point, so that we know the direction in the converted
		// space which we need to extend in. Since this will be coplanar to
		// the circle, we can cross product norm_1 with the vector from the
		// source to the centre to get something facing the right way.
		Vector3 hyp_vector = ~(norm_1 * (centre-from));

		// Now we calculate the hyperbolic distance between eye and point
		// If one or the other of our points lies "behind" the centre, relative
		// to the direction vector, we need to negate our square root, as it is
		// a simplification of a cosine, which negates for angles over pi/2.
		float frommult = (fabs(dir.x) > fabs(dir.y) ? dir.x * (from.x - centre.x) : dir.y * (from.y - centre.y)) > 0 ? 1 : -1;
		float tomult = (fabs(dir.x) > fabs(dir.y) ? dir.x * (to.x - centre.x) : dir.y * (to.y - centre.y)) > 0 ? 1 : -1;
		float hyp_dist = fabs(log((from.z/to.z) * ((radius-tomult*sqrt(radius*radius - to.z*to.z))/(radius-frommult*sqrt(radius*radius - from.z*from.z)))));

		// Multiply by the hyperbolic facing vector and add to eye to get our
		// projected co-ordinates!
		return from + hyp_vector * hyp_dist;
	} else {
		// We're assuming that we're simply in a vertical line segment. This
		// saves much culculation which will be getting vastly innaccurate
		// anyway. Note that the log will negate if we swap the vectors, so
		// we don't even need to work out direction.
		float hyp_dist = log(from.z/to.z);
		Vector3 hyp_vector = Vector3(0,0,1);
		return from - hyp_vector * hyp_dist;
	}
}

// Not strictly adding this vector, but assuming a geodesic path starting out
// in dir and finishing after walking length(dir). Dir is modified to the new
// facing vector after movement.
Point3 embeddingAddVector_hyperbolic(const Point3 &source, Vector3 &dir) {
	return embeddingAddVector_hyperbolic(source, dir, (float)dir);
}

Point3 embeddingAddVector_hyperbolic(const Point3 &source, Vector3 &dir, const float distance) {
	// First, find the centre of the circle which is coplanar to source and
	// dir. norm_1 is parallel to the z-axis and perpendicular to dir, so if
	// we cross with dir, we point towards the centre.
	Vector3 norm_1 = dir * Vector3(0,0,1);
	Vector3 norm = ~(norm_1 * dir);
	if (fabs(norm.z) > 0.0001 * source.z) {
		printf("Case 1\n");
		float radius = source.z/norm.z;
		Point3 centre = source - norm * radius;
		Vector3 norm_proj = fabs(norm.x) + fabs(norm.y) > 0 ? ~Vector3(norm.x, norm.y, 0) : dir;

		// Now we need the angle around this circle at which source resides. We can
		// get the cosine by noting that norm dot norm_proj = cos theta.
		float angle = acos(norm & norm_proj);

		// Work out how far to go. If dir.z is positive, we need to increase theta
		// and we make distance positive.
		//	float distance = (dir.z > 0 ? 1 : -1) * (float)dir;
		float realdistance = (dir.z > 0 ? 1 : -1) * distance;

		// Now we can use this neato formula
		float newAngle = 2*atan(exp(realdistance)*tan(angle/2));

		// Converting back, we see that...
		Point3 newPoint = Point3(
				centre.x + norm_proj.x*cos(newAngle)*radius,
				centre.y + norm_proj.y*cos(newAngle)*radius,
				sin(newAngle)*radius);

		// And we can work out our new direction vector, too, preserving length
		dir = ((float)dir) * ~((newPoint - centre) * norm_1);

		return newPoint;
	} else {
		printf("Case 2\n");
		// Special case for almost vertical lines...
		Point3 newPoint = Point3(source.x, source.y, 0) + Vector3(0,0,1) * (float)exp((dir.z > 0 ? 1 : -1) * distance) * source.z;
		return newPoint;
	}
}

void glVertex3f_hyperbolic(Point3 eye, Point3 target) {
	Point3 eucLoc = embeddingToLocalPoint_hyperbolic(eye, target);
	glVertex3f(eucLoc.x, eucLoc.y, eucLoc.z);
}

void glVertex3f_hyperbolic(Point3 eye, float x, float y, float z) {
	glVertex3f_hyperbolic(eye, Point3(x, y, z));
}

void gluLookAt_hyperbolic(Point3 eye, Vector3 forward, Vector3 up) {
	gluLookAt(eye.x, eye.y, eye.z, eye.x + forward.x, eye.y + forward.y, eye.z + forward.z, up.x, up.y, up.z);
}

void gluLookAt_hyperbolic(Point3 eye, Point3 target, Vector3 up) {
	gluLookAt_hyperbolic(eye, embeddingToLocalPoint_hyperbolic(eye, target), up);
}

}
