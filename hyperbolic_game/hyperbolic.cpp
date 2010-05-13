#include <math.h>
#include <stdio.h>
#include <iostream>
#include <GL/glfw.h>
#include "point.hpp"
#include "hyperbolic.hpp"

namespace bucko {
using namespace std;

const float neginf = log(0);
const float inf = -neginf;

int debug;

// Returns (inf, inf, 0) or (x, y, 0)
// Also leaves circle_plane_norm set to a normal to the plane on which the
// circle resides.
Vector3 circle_plane_norm = Vector3(0, 0, 0), vec_to_centre = Vector3(0, 0, 0);
Point3 findCircleCentre(const Point3 &point1, const Point3 &point2) {
	Vector3 eucDirection = point1.Point3::directionTo(point2);
	// And the midpoint of the Euclidean line segment connecting the two points.
	Point3 mid = point1.Point3::addVector(eucDirection / 2.0f);

	// Before we do anything else, avoid wasteful calculations for infinite or
	// near-infinite circles.
	if (fabs(eucDirection.z) > fabs(eucDirection.x) * 1000.0 * mid.z && fabs(eucDirection.z) > fabs(eucDirection.y) * 1000.0 * mid.z) {
		return Point3(inf, inf, 0);
	}

	// circle_plane_norm is a vector perpendicular to the dir, and parallel
	// to the Z plane.
	// It doesn't matter where it points; all we need are those properties.
	Vector3 planar_norm = Vector3(0,0,1);
	circle_plane_norm = eucDirection * planar_norm;

	// A vector perpendicular to perp_1 and to dir must point from mid towards
	// the centre of the Euclidean circle with this line as a chord. (Or away)
	Vector3 vec_to_centre = eucDirection * circle_plane_norm;

	// Solve the linear equation mid + vec_to_centre * copies = centre
	// using the z co-ordinates, for copies.
	float copies = mid.z / vec_to_centre.z;

	// Now we've got the centre!
	return mid - vec_to_centre * copies;
}

Point3 findCircleCentre(const Point3 &source, const Vector3 &eucDirection) {
	// Before we do anything else, avoid wasteful calculations for infinite or
	// near-infinite circles.
	if (fabs(eucDirection.z) > fabs(eucDirection.x) * 1000.0 * source.z && fabs(eucDirection.z) > fabs(eucDirection.y) * 1000.0 * source.z) {
		return Point3(inf, inf, 0);
	}

	// circle_plane_norm is a vector perpendicular to the dir, and parallel
	// to the Z plane.
	// It doesn't matter where it points; all we need are those properties.
	Vector3 planar_norm = Vector3(0,0,1);
	circle_plane_norm = eucDirection * planar_norm;

	// A vector perpendicular to perp_1 and to dir must point from mid towards
	// the centre of the Euclidean circle with this line as a chord. (Or away)
	vec_to_centre = eucDirection * circle_plane_norm;

	// Solve the linear equation mid + vec_to_centre * copies = centre
	// using the z co-ordinates, for copies.
	float copies = source.z / vec_to_centre.z;

	// Now we've got the centre!
	return source.Point3::addVector(vec_to_centre * -copies);
}

Vector3 HyperbolicPoint3::directionTo(const HyperbolicPoint3 &target) const {
	Point3 centre = findCircleCentre(*this, target);
	// Here we special case. If the returned circle centre is at infinite
	// distance, we only need to resolve along a vertical Euclidean line.
	if (centre.x != inf) {
		// Find the Euclidean direction vector
		Vector3 eucDirection = Point3::directionTo(target);

		// This point is on the circumference of the circle, hence is at
		// distance radius from the centre.
		Vector3 vec_to_centre = Point3::directionTo(centre);
		float radius = vec_to_centre.length();

		// We now need to find the hyperbolic direction from relativeTo to
		// this. This is a tangent to the circle at this, so we can cross the
		// plane norm with the centre vector.
		// This vector may be backwards; we have no guarantees about direction
		// of vec_to_centre. We also don't know its length, though we
		// immediately calculate it.
		Vector3 hyp_vector = circle_plane_norm * vec_to_centre;
		float hyp_vector_length = hyp_vector.length();

		// If the hyperbolic vector does happen to be backwards, make the length
		// negative for later.
		//if (hyp_vector.z * eucDirection.z < -1) hyp_vector_length *= -1;

		// Now we calculate the hyperbolic distance between eye and point. This
		// is equal to |log((csc(t_1) - cot(t_1))/(csc(t_2) - cot(t_2)))|,
		// where t_i is the angle from centre + radius * ~dir to point i.
		// By some rearrangement noting that sin t_i = z_i/radius and
		// cos t_i = sqrt(1 - sin^2(t_i)) * sgn(x_i) * sgn(d_i)
		// We get |log((sin(t_2)/sin(t_1))*(1-cos(t_1))/(1-cos(t_2)))|
		//      = |log((z_2/z_1)*(1-cos(t_1))/(1-cos(t_2)))|
		float sin_this = z / radius;
		float sin_target = target.z / radius;
		float cos_this, cos_target;
		if (sin_this > 1.0) sin_this = 1.0;
		if (sin_target > 1.0) sin_target = 1.0;
		if (fabs(eucDirection.x) > fabs(eucDirection.y)) {
			// x is bigger; use it for comparison.
			// Note the sign condition is equivalent to being in the same
			// direction as the direction vector.
			// We already worked out centre - *this, so we use it here.
			cos_this = sqrt(1 - sin_this * sin_this) * (eucDirection.x * vec_to_centre.x < 0 ? 1 : -1);
			cos_target = sqrt(1 - sin_target * sin_target) * (eucDirection.x * (centre.x - target.x) < 0 ? 1 : -1);
		} else {
			cos_this = sqrt(1 - sin_this * sin_this) * (eucDirection.y * vec_to_centre.y < 0 ? 1 : -1);
			cos_target = sqrt(1 - sin_target * sin_target) * (eucDirection.y * (centre.y - target.y) < 0 ? 1 : -1);
		}
		if (debug) cout << "cosines " << cos_this << " " << cos_target << "." << endl;
		float hyp_dist = log(
			(target.z / z) * (1 - cos_this)/(1 - cos_target)
		);

		// Multiply by the hyperbolic facing vector and add to eye to get our
		// projected co-ordinates!
		return hyp_vector * hyp_dist / hyp_vector_length;
	} else {
		// We're assuming that we're simply in a vertical line segment. This
		// saves much culculation which will be getting vastly innaccurate
		// anyway. Note that the log will negate if we swap the vectors, so
		// we don't even need to work out direction.
		float hyp_dist = log(z/target.z);
		Vector3 hyp_vector = Vector3(0,0,1);
		return hyp_vector * hyp_dist;
	}
}

Point3 HyperbolicPoint3::disEmbed(const HyperbolicPoint3 &relativeTo) const {
	return relativeTo.Point3::addVector(relativeTo.directionTo(*this));
}

// Not strictly adding this vector, but assuming a geodesic path starting out
// in dir and finishing after walking length(dir). Dir is modified to the new
// facing vector after movement.
HyperbolicPoint3 HyperbolicPoint3::addVector(Vector3 &dir) const {
	return addVector(dir, dir.length());
}

HyperbolicPoint3 HyperbolicPoint3::addVector(Vector3 &dir, const float distance) const {
	Point3 centre = findCircleCentre(*this, dir);
	if (centre.x != inf) {
		// Project the vector to the centre down to a unit vector on the Z plane
		float vec_to_centre_length = vec_to_centre.length();
		float radius = fabs(z * vec_to_centre_length / vec_to_centre.z);
		Vector3 dir_proj = ~Vector3(dir.x, dir.y, 0);
		printf("Vec to Centre: (%f, %f, %f)\n", vec_to_centre.x, vec_to_centre.y, vec_to_centre.z);
		printf("Centre: (%f, %f, %f)\n", centre.x, centre.y, centre.z);

		// Now we need the angle around this circle at which source resides.
		// We can get the cosine by noting that vec_to_centre dot dir_proj =
		// cos theta.
		float angle = acos((vec_to_centre & dir_proj) / vec_to_centre_length);

		// Work out how far to go. Note that negating the direction negates the
		// vec_to_centre vector too, hence we need a sign check on dir.z to
		// work out what's cracking off with the angle.
		float realdistance = distance;

		// Now we can use this neato formula
		float newAngle = 2*atan(exp(realdistance)*tan(angle/2));
		printf("New %f old %f rad %f\n", newAngle, angle, radius);

		// Converting back, we see that...
		HyperbolicPoint3 newPoint = HyperbolicPoint3(
				centre.x - dir_proj.x*cos(newAngle)*radius,
				centre.y - dir_proj.y*cos(newAngle)*radius,
				sin(newAngle)*radius);

		// And we can work out our new direction vector, too, preserving length
		dir = dir.length() * ~(newPoint.Point3::directionTo(centre) * circle_plane_norm);

		return newPoint;
	} else {
		printf("Case 2\n");
		// Special case for almost vertical lines...
		HyperbolicPoint3 newPoint = HyperbolicPoint3(x, y, (float)exp((dir.z > 0 ? 1 : -1) * distance) * z);
		return newPoint;
	}
}

HyperbolicPlane3::HyperbolicPlane3(const HyperbolicPoint3 &centre, const float eucRadius) : centre(centre), radius(eucRadius) {
}

HyperbolicPlane3::HyperbolicPlane3(const HyperbolicPoint3 &point1, const HyperbolicPoint3 &point2, const HyperbolicPoint3 &point3) : centre(HyperbolicPoint3(0,0,0)) {
	// Elementary geometry
	Vector3 vector1 = point1.directionTo(point2);
	Vector3 vector2 = point1.directionTo(point3);
	Vector3 vec_to_centre = vector1 * vector2;
	centre.x = point1.x - vec_to_centre.x * (point1.z / vec_to_centre.z);
	centre.y = point1.y - vec_to_centre.y * (point1.z / vec_to_centre.z);
	centre.z = point1.z - vec_to_centre.z * (point1.z / vec_to_centre.z);
	radius = centre.Point3::directionTo(point1).length();
}

// Find the shortest local vector to a hyperbolic plane
Vector3 HyperbolicPoint3::directionTo(const HyperbolicPlane3 &plane) const {
	// Start by finding the (Euclidean) direction to the (Euclidean) centre.
	Vector3 euc_this_to_centre = Point3::directionTo(plane.centre);

	// Is the centre basically straight down? If so we can calculate
	// immediately.
	if (fabs(euc_this_to_centre.z) > fabs(euc_this_to_centre.x) * 1000.0 && fabs(euc_this_to_centre.z) > fabs(euc_this_to_centre.y) * 1000.0) {
		return Vector3(0, 0, log(plane.radius / z));
	}
	
	// Find a unit vector along the boundary pointing in the right
	// direction.
	Vector3 proj_this_to_centre = Vector3(euc_this_to_centre.x, euc_this_to_centre.y, 0);
	Vector3 unit_proj_this_to_centre = ~proj_this_to_centre;

	// Lots of trig to get this one
	float cosine = 2 * plane.radius * (proj_this_to_centre & unit_proj_this_to_centre) / (plane.radius * plane.radius + (euc_this_to_centre & euc_this_to_centre));

	// New algo
/*	HyperbolicPoint3 arc_centre = HyperbolicPoint3(
		unit_proj_this_to_centre.x * plane.radius / cosine,
		unit_proj_this_to_centre.y * plane.radius / cosine,
		0);
	Vector3 circle_norm = unit_proj_this_to_centre * Vector3(0, 0, 1);
	Vector3 dir_to_centre = Point3::directionTo(arc_centre);
	Vector3 direction = dir_to_centre * circle_norm;
	direction = direction * (*/
	HyperbolicPoint3 closest_point = HyperbolicPoint3(
	        plane.centre.x - plane.radius * unit_proj_this_to_centre.x * cosine,
	        plane.centre.y - plane.radius * unit_proj_this_to_centre.y * cosine,
	    /*0=plane.centre.z*/ plane.radius * sqrt(1 - cosine * cosine));
	cout << closest_point;

	debug = 1;
	Vector3 direction = directionTo(closest_point);
	debug = 0;
	return direction;
		//			up.z = (float)(dir_grav_to_eye) * (((dir_grav_to_eye & dir_grav_to_eye) + grav_radius*grav_radius)/(2*(inf_grav_to_eye & inf_grav_to_eye))-1);
}

HyperbolicPoint3::HyperbolicPoint3(float x, float y, float z) : Point3(x, y, z) {
}

void glVertex3f(const HyperbolicPoint3 &eye, const HyperbolicPoint3 &target) {
	Point3 eucLoc = target.disEmbed(eye);
	::glVertex3f(eucLoc.x, eucLoc.y, eucLoc.z);
}

void glVertex3f(const HyperbolicPoint3 &eye, const float x, const float y, const float z) {
	glVertex3f(eye, HyperbolicPoint3(x, y, z));
}

void gluLookAt(const HyperbolicPoint3 &eye, const Vector3 &forward, const Vector3 &up) {
	::gluLookAt(eye.x, eye.y, eye.z, eye.x + forward.x, eye.y + forward.y, eye.z + forward.z, up.x, up.y, up.z);
}

void gluLookAt(const HyperbolicPoint3 &eye, const HyperbolicPoint3 &target, const Vector3 &up) {
	gluLookAt(eye, eye.directionTo(target), up);
}

}

