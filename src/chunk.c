#include <stdlib.h>

#include "../include/glad/glad.h"
#include "../include/chunk.h"

static Chunk* g_chunks;
static Mesh* g_chunk_meshes;
static uint g_chunk_count;

Mesh test;

void Chunks_init() {
	g_chunk_count = 1;

	float verts[] = {
		-0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f, -0.5f, 
		 0.5f,  0.5f, -0.5f, 
		 0.5f,  0.5f, -0.5f, 
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 

		-0.5f, -0.5f,  0.5f, 
		 0.5f, -0.5f,  0.5f, 
		 0.5f,  0.5f,  0.5f, 
		 0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f, 

		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 

		 0.5f,  0.5f,  0.5f, 
		 0.5f,  0.5f, -0.5f, 
		 0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f,  0.5f, 
		 0.5f,  0.5f,  0.5f, 

		-0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f,  0.5f, 
		 0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f, -0.5f, 

		-0.5f,  0.5f, -0.5f, 
		 0.5f,  0.5f, -0.5f, 
		 0.5f,  0.5f,  0.5f, 
		 0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f, 
	};

	float uv[] = {
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		1.f, 1.f,
		0.f, 1.f,
		0.f, 0.f,

		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		1.f, 1.f,
		0.f, 1.f,
		0.f, 0.f,

		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f,
		0.f, 1.f,
		0.f, 0.f,
		1.f, 0.f,

		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f,
		0.f, 1.f,
		0.f, 0.f,
		1.f, 0.f,

		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		1.f, 1.f,
		0.f, 1.f,
		0.f, 0.f,

		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		1.f, 1.f,
		0.f, 1.f,
		0.f, 0.f
	};

	test = Mesh_create((Vector3*) verts, (Vector2*) uv, 36);
	g_chunk_meshes = &test;
}

void Chunk_generateMesh(uint chunk_id) {
}

void Chunks_draw() {
	Matrix transform;

	Mesh_bind(test);
	glDrawArrays(GL_TRIANGLES, 0, test.vertex_count);
}
