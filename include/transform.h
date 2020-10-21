#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "types.h"

typedef float Transform[16];

void Transform_rotateX(Transform transform, float angle);
void Transform_rotateY(Transform transform, float angle);
void Transform_rotateZ(Transform transform, float angle);

void Transform_scale(Transform transform, Vec3 scale);
void Transform_translate(Transform transform, Vec3 translation);

#endif
