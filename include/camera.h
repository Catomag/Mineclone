#ifndef _CAMERA_H_
#define _CAMERA_H_

#define RAYMATH_STANDALONE
#include <raymath.h>

typedef struct {
	Vector3 pos;
	Vector3 front;
	float pitch;
	float yaw;
	Matrix view;
	Matrix projection;
} Camera;

Camera Camera_create(float fov, float width, float height, float near, float far);
void Camera_set(Camera* cam, float fov, float width, float height, float near, float far);
void Camera_update(Camera* cam, Vector3 up);

#endif
