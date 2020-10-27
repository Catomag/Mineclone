#ifndef MATRIX_H
#define MATRIX_H

#include "types.h"

typedef float Matrix[16];

// ALWAYS
// scale
// rotate
// translate

void Matrix_identity(Matrix mat);
void Matrix_multiply(Matrix a, Matrix b);
void Matrix_print(Matrix mat);

void Matrix_projection(Matrix a, float fov, float aspect_ratio, float near_distance, float far_distance);

void Matrix_translate(Matrix mat, Vec3 translation);
void Matrix_scale(Matrix mat, Vec3 scale);

void Matrix_rotateX(Matrix mat, float angle);
void Matrix_rotateY(Matrix mat, float angle);
// NOT IMPLEMENTED
void Matrix_rotateZ(Matrix mat, float angle);
// NOT IMPLEMENTED
void Matrix_rotate(Matrix mat, Vec3 angles);

#endif
