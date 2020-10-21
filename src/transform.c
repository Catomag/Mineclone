#include <stdio.h>
#include "../include/transform.h"

void Transform_identity(Transform transform) {
	for(int i = 0; i < 16; i++)
		transform[i] = 0.f;

	transform[0]  = 1.f;
	transform[5]  = 1.f;
	transform[10] = 1.f;
	transform[15] = 1.f;
}

void Transform_multiply(Transform res, Transform a, Transform b) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			int index = i*4 + j;
			float sum = 0;

			for(int k = 0; k < 4; k++)
				sum += a[index] * b[k*4 + i];

			res[index] = sum;
		}
	}
}

void Transform_print(Transform transform) {
	for(int i = 0; i < 4; i++)
		printf("[ %.2f, %.2f, %.2f, %.2f ]\n", transform[i*4], transform[i*4 + 1], transform[i*4 + 2], transform[i*4 + 3]);
}

void Transform_translate(Transform transform, Vec3 translation) {
	transform[3]  = translation.x;
	transform[7]  = translation.y;
	transform[11] = translation.z;
}

void Transform_scale(Transform transform, Vec3 scale) {
}


void Transform_rotateX(Transform transform, float angle) {
}

void Transform_rotateY(Transform transform, float angle) {
}

void Transform_rotateZ(Transform transform, float angle) {
}
