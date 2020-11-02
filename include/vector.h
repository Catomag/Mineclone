#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
	float x;
	float y;
	float z;
} Vector3;

typedef struct {
	float x;
	float y;
} Vector2;

Vector3 Vector3_add(Vector3 a, Vector3 b);
Vector3 Vector3_minus(Vector3 a, Vector3 b);

Vector3 Vector3_mult(Vector3 a, float b);

Vector3 Vector3_cross(Vector3 a, Vector3 b);
Vector3 Vector3_norm(Vector3 vec);

#endif
