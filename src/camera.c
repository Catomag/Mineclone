#include "../include/glad/glad.h"
#include "../include/camera.h"

Camera Camera_create(float fov, float width, float height, float near, float far) {
	Camera cam;
	cam.yaw = 0;
	cam.pitch = 0;
	cam.pos = (Vector3) { 0 };
	cam.view = MatrixIdentity();
	cam.projection = MatrixIdentity();
	cam.projection = MatrixPerspective(fov, width / height, near, far);

	return cam;
}

void Camera_set(Camera* cam, float fov, float width, float height, float near, float far) {
	cam->projection = MatrixIdentity();
	cam->projection = MatrixPerspective(fov, width / height, near, far);
}

void Camera_update(Camera* cam, Vector3 up) {
	if(cam->pitch > (89.0f * PI / 180.f))
		cam->pitch = (89.0f * PI / 180.f);

	if(cam->pitch < -(89.0f * PI / 180.f))
		cam->pitch = -(89.0f * PI / 180.f);

	cam->front.x = cos(cam->yaw) * cos(cam->pitch);
	cam->front.y = sin(cam->pitch);
	cam->front.z = sin(cam->yaw) * cos(cam->pitch);
	cam->front = Vector3Normalize(cam->front);

	cam->view = MatrixLookAt(cam->pos, Vector3Add(cam->pos, cam->front), up);
}
