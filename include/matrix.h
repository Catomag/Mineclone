#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "types.h"

typedef float Matrix[16];

// ALWAYS
// scale
// rotate
// translate

void Matrix_identity(Matrix mat);
void Matrix_multiply(Matrix a, Matrix b);
void Matrix_print(Matrix mat);

void Matrix_translate(Matrix mat, Vec3 translation);
void Matrix_scale(Matrix mat, Vec3 scale);

void Matrix_rotateX(Matrix mat, float angle);
void Matrix_rotateY(Matrix mat, float angle);
void Matrix_rotateZ(Matrix mat, float angle);
void Matrix_rotate(Matrix mat, Vec3 angles);

#endif
