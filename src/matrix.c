#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "../include/vector.h"
#include "../include/matrix.h"

#define PI 3.14159
#define HALF_PI 1.57079

void Matrix_identity(Matrix mat) {
	for(int i = 0; i < 16; i++)
		mat[i] = 0.f;

	mat[0]  = 1.f;
	mat[5]  = 1.f;
	mat[10] = 1.f;
	mat[15] = 1.f;
}

//------------------------------------------------------------------------

void Matrix_multiply(Matrix a, Matrix b) {
	Matrix res;

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			int index = i*4 + j;
			float sum = 0;

			for(int k = 0; k < 4; k++)
				sum += a[i*4 + k] * b[k*4 + j];

			res[index] = sum;
		}
	}

	for(int i = 0; i < 16; i++)
		a[i] = res[i];
}

//------------------------------------------------------------------------

void Matrix_print(Matrix mat) {
	for(int i = 0; i < 4; i++)
		printf("[ %.2f, %.2f, %.2f, %.2f ]\n", mat[i*4], mat[i*4 + 1], mat[i*4 + 2], mat[i*4 + 3]);
}

//------------------------------------------------------------------------

void Matrix_perspective(Matrix mat, float fov, float aspect_ratio, float near_distance, float far_distance) {
	assert(fov > 0 && aspect_ratio != 0);

	for(int i = 0; i < 16; i++)
		mat[i] = 0.f;

	mat[0]   = 1.f / (aspect_ratio * tan(fov/2));
	mat[5]   = 1.f / tan(fov/2);
	mat[10] -= (far_distance + near_distance) / (far_distance - near_distance);
	mat[11] -= 1.f;
	mat[14] -= 2.f * far_distance * near_distance / (far_distance - near_distance);
}

//------------------------------------------------------------------------

void Matrix_look_at(Matrix mat, Vector3 pos, Vector3 target, Vector3 up) {
	Vector3 direction;
	Vector3 right;
	Vector3 camera_up;

	direction = Vector3_minus(pos, target);
	direction = Vector3_norm(direction);

	right = Vector3_cross(up, direction);
	right = Vector3_norm(right);

	camera_up = Vector3_cross(direction, right);

	Matrix look_at;
	Matrix_identity(look_at);

	look_at[0]  = right.x;
	look_at[4]  = right.y;
	look_at[8]  = right.z;

	look_at[1]  = up.x;
	look_at[5]  = up.y;
	look_at[9]  = up.z;

	look_at[2]  = direction.x;
	look_at[6]  = direction.y;
	look_at[10] = direction.z;

	Matrix_translate(mat, Vector3_mult(pos, -1));
	Matrix_multiply(mat, look_at);
}

//------------------------------------------------------------------------

void Matrix_translate(Matrix mat, Vector3 translation) {
	Matrix temp;
	Matrix_identity(temp);

	temp[12] = translation.x;
	temp[13] = translation.y;
	temp[14] = translation.z;

	Matrix_multiply(mat, temp);
}

//------------------------------------------------------------------------

void Matrix_scale(Matrix mat, Vector3 scale) {
	Matrix temp;
	Matrix_identity(temp);

	temp[0]  = scale.x;
	temp[5]  = scale.y;
	temp[10] = scale.z;

	Matrix_multiply(mat, temp);
}

//------------------------------------------------------------------------

void Matrix_rotateX(Matrix mat, float angle) {
	Matrix temp;
	Matrix_identity(temp);

	temp[5]  =  cos(angle);
	temp[6]  =  sin(angle);
	temp[9]  = -temp[6];
	temp[10] =  temp[5];

	Matrix_multiply(mat, temp);
}

//------------------------------------------------------------------------

void Matrix_rotateY(Matrix mat, float angle) {
	Matrix temp;
	Matrix_identity(temp);

	temp[0]  =  cos(angle);
	temp[8]  =  sin(angle);
	temp[2]  = -temp[8];
	temp[10] =  temp[0];

	Matrix_multiply(mat, temp);
}

//------------------------------------------------------------------------

void Matrix_rotateZ(Matrix mat, float angle) {
}
