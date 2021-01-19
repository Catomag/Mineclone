#ifndef _MESH_H_
#define _MESH_H_
#include "types.h"

#define RAYMATH_STANDALONE
#include <raymath.h>

typedef struct {
	uint vao;
	uint vbo;
	uint vertex_count;
	Vector3* vertices;
	Vector2* uvs;
} Mesh;

Mesh Mesh_create(Vector3* vertices, Vector2* uvs, uint vertex_count);
void Mesh_update(Mesh* mesh, Vector3* vertices, Vector2* uvs, uint vertex_count);
void Mesh_destroy(Mesh* mesh);

void Mesh_bind(Mesh mesh);
#endif
