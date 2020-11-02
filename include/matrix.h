#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include "types.h"

typedef float Matrix[16];

// ALWAYS
// scale
// rotate
// translate

void Matrix_identity(Matrix mat);
void Matrix_multiply(Matrix a, Matrix b);
void Matrix_print(Matrix mat);

void Matrix_perspective(Matrix a, float fov, float aspect_ratio, float near_distance, float far_distance);
void Matrix_look_at(Matrix mat, Vector3 pos, Vector3 target, Vector3 up);

void Matrix_translate(Matrix mat, Vector3 translation);
void Matrix_scale(Matrix mat, Vector3 scale);

void Matrix_rotateX(Matrix mat, float angle);
void Matrix_rotateY(Matrix mat, float angle);
// NOT IMPLEMENTED
void Matrix_rotateZ(Matrix mat, float angle);
// NOT IMPLEMENTED
void Matrix_rotate(Matrix mat, Vector3 angles);

#endif
