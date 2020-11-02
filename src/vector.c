#include <math.h>
#include "../include/vector.h"

Vector3 Vector3_add(Vector3 a, Vector3 b) {
	Vector3 c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;

	return c;
}

// Vector a minus Vector b
Vector3 Vector3_minus(Vector3 a, Vector3 b) {
	Vector3 c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;

	return c;
}

Vector3 Vector3_mult(Vector3 a, float b) {
	a.x *= b;
	a.y *= b;
	a.z *= b;

	return a;
}

Vector3 Vector3_cross(Vector3 a, Vector3 b) {
	Vector3 c;
	c.x = a.y*b.z - a.z*b.y;
	c.y = a.z*b.x - a.x*b.z;
	c.z = a.x*b.y - a.y*b.x;

	return c;
}

Vector3 Vector3_norm(Vector3 vec) {
	Vector3 c;
	float length = sqrt((vec.x*vec.x) + (vec.y + vec.y) + (vec.z + vec.z));

	c.x = vec.x/length;
	c.y = vec.y/length;
	c.z = vec.z/length;
	return c;
}
