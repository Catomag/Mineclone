#include <stdlib.h>

#include "../include/glad/glad.h"
#include "../include/mesh.h"
#include "../include/debug.h"

Mesh Mesh_create(Vector3* vertices, Vector2* uvs, uint vertex_count) {
	Mesh mesh;

	mesh.vertices = malloc(sizeof(Vector3) * vertex_count);
	mesh.uvs = malloc(sizeof(Vector2) * vertex_count);
	mesh.vertex_count = vertex_count;

	for(uint i = 0; i < mesh.vertex_count; i++) {
		mesh.vertices[i] = vertices[i];
		mesh.uvs[i] = uvs[i];
	}

	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);

	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * mesh.vertex_count, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * mesh.vertex_count, mesh.vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.vertex_count, sizeof(float) * 2 * mesh.vertex_count, mesh.uvs);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void*) (sizeof(float) * 3 * mesh.vertex_count));
	glEnableVertexAttribArray(1);

	return mesh;
}

void Mesh_update(Mesh* mesh, Vector3* vertices, Vector2* uvs, uint vertex_count) {
	mesh->vertices = realloc(mesh->vertices, sizeof(Vector3) * vertex_count);
	mesh->uvs = realloc(mesh->uvs, sizeof(Vector2) * vertex_count);
	mesh->vertex_count = vertex_count;

	for(uint i = 0; i < mesh->vertex_count; i++) {
		mesh->vertices[i] = vertices[i];
		mesh->uvs[i] = uvs[i];
	}

	glBindVertexArray(mesh->vao);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * mesh->vertex_count, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * mesh->vertex_count, mesh->vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->vertex_count, sizeof(float) * 2 * mesh->vertex_count, mesh->uvs);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void*) (sizeof(float) * 3 * mesh->vertex_count));
	glEnableVertexAttribArray(1);
}

void Mesh_destroy(Mesh* mesh) {
	free(mesh->vertices);
	free(mesh->uvs);
	glDeleteVertexArrays(1, &mesh->vao);
	glDeleteBuffers(1, &mesh->vbo);
}

void Mesh_bind(Mesh mesh) {
	glBindVertexArray(mesh.vao);
}
