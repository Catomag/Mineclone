#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "types.h"

typedef float Transform[16];

// ALWAYS
// scale
// rotate
// translate

void Transform_identity(Transform transform);
void Transform_multiply(Transform res, Transform a, Transform b);
void Transform_print(Transform transform);

void Transform_translate(Transform transform, Vec3 translation);
void Transform_scale(Transform transform, Vec3 scale);

void Transform_rotateX(Transform transform, float angle);
void Transform_rotateY(Transform transform, float angle);
void Transform_rotateZ(Transform transform, float angle);

#endif
