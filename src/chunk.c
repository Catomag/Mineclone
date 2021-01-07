#include <stdlib.h>

#include "../include/glad/glad.h"
#include "../include/chunk.h"

static Chunk* g_chunks;
static Mesh* g_chunk_meshes;
static uint g_chunk_count;

Mesh test;

void Chunks_init() {
	g_chunk_count = 1;

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
}

void Chunk_generateMesh(uint chunk_id) {

	unsigned int quad_count = 0;
	for(short x = 0; x < CHUNK_LENGTH; x++) {
		for(short y = 0; y < CHUNK_LENGTH; y++) {
			for(short z = 0; z < CHUNK_HEIGHT; z++) {
				unsigned int i = (z * CHUNK_HEIGHT + y) * CHUNK_LENGTH + x;
				if(g_chunks[chunk_id].data[i] != 0) {
					unsigned int j = 0;

					// add mesh right
					j = (z * CHUNK_HEIGHT + y) * CHUNK_LENGTH + (x + 1);
					quad_count += (g_chunks[chunk_id].data[j] != 0);

					// add mesh left
					j = (z * CHUNK_HEIGHT + y) * CHUNK_LENGTH + (x - 1);
					quad_count += (g_chunks[chunk_id].data[j] != 0);

					// add mesh left
					j = (z * CHUNK_HEIGHT + (y + 1)) * CHUNK_LENGTH + x;
					quad_count += (g_chunks[chunk_id].data[j] != 0);

					// add mesh left
					j = (z * CHUNK_HEIGHT + (y - 1)) * CHUNK_LENGTH + x;
					quad_count += (g_chunks[chunk_id].data[j] != 0);

					// add mesh left
					j = ((z + 1) * CHUNK_HEIGHT + y) * CHUNK_LENGTH + x;
					quad_count += (g_chunks[chunk_id].data[j] != 0);

					// add mesh left
					j = ((z - 1) * CHUNK_HEIGHT + y) * CHUNK_LENGTH + x;
					quad_count += (g_chunks[chunk_id].data[j] != 0);
				}
			}
		}
	}
	
	// allocate all the quads
	Vector3 verts[quad_count * 6];
	Vector2 uvs[quad_count * 6];
	unsigned int vert_count = 0;

	// actually compute them
	for(short x = 0; x < CHUNK_LENGTH; x++) {
		for(short y = 0; y < CHUNK_HEIGHT; y++) {
			for(short z = 0; z < CHUNK_LENGTH; z++) {
				unsigned int i = (y * CHUNK_HEIGHT + z) * CHUNK_LENGTH + x;
				if(g_chunks[chunk_id].data[i] != 0) {
					unsigned int j = 0;

					// add mesh right
					j = (z * CHUNK_HEIGHT + y) * CHUNK_LENGTH + (x + 1);
					if(j < 0 || j > CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT) {
						if(g_chunks[chunk_id].data[j] != 0) {
							verts[vert_count] 	  = (Vector3) { x + 1.f, y + 1.f, z + 1.f };
							verts[vert_count + 1] = (Vector3) { x + 1.f, y + 1.f, z - 1.f };
							verts[vert_count + 2] = (Vector3) { x + 1.f, y - 1.f, z - 1.f };
							verts[vert_count + 3] = (Vector3) { x + 1.f, y - 1.f, z - 1.f };
							verts[vert_count + 4] = (Vector3) { x + 1.f, y - 1.f, z + 1.f };
							verts[vert_count + 5] = (Vector3) { x + 1.f, y + 1.f, z + 1.f };

							uvs[vert_count]  	= (Vector2) { 1.f, 0.f };
							uvs[vert_count + 1] = (Vector2) { 1.f, 1.f };
							uvs[vert_count + 2] = (Vector2) { 0.f, 1.f };
							uvs[vert_count + 3] = (Vector2) { 0.f, 1.f };
							uvs[vert_count + 4] = (Vector2) { 0.f, 0.f };
							uvs[vert_count + 5] = (Vector2) { 1.f, 0.f };
							vert_count += 6;
						}
					}

					// add mesh left
					j = (z * CHUNK_HEIGHT + y) * CHUNK_LENGTH + (x - 1);
					if(j < 0 || j > CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT) {
						if(g_chunks[chunk_id].data[j] != 0) {
							verts[vert_count] 	  = (Vector3) { x - 1.f, y + 1.f, z + 1.f };
							verts[vert_count + 1] = (Vector3) { x - 1.f, y + 1.f, z - 1.f };
							verts[vert_count + 2] = (Vector3) { x - 1.f, y - 1.f, z - 1.f };
							verts[vert_count + 3] = (Vector3) { x - 1.f, y - 1.f, z - 1.f };
							verts[vert_count + 4] = (Vector3) { x - 1.f, y - 1.f, z + 1.f };
							verts[vert_count + 5] = (Vector3) { x - 1.f, y + 1.f, z + 1.f };

							uvs[vert_count]  	= (Vector2) { 1.f, 0.f };
							uvs[vert_count + 1] = (Vector2) { 1.f, 1.f };
							uvs[vert_count + 2] = (Vector2) { 0.f, 1.f };
							uvs[vert_count + 3] = (Vector2) { 0.f, 1.f };
							uvs[vert_count + 4] = (Vector2) { 0.f, 0.f };
							uvs[vert_count + 5] = (Vector2) { 1.f, 0.f };
							vert_count += 6;
						}
					}

					// add mesh up
					j = (z * CHUNK_HEIGHT + (y + 1)) * CHUNK_LENGTH + x;
					if(j < 0 || j > CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT) {
						if(g_chunks[chunk_id].data[j] != 0) {
							verts[vert_count] 	  = (Vector3) { x - 1.f, y + 1.f, z - 1.f };
							verts[vert_count + 1] = (Vector3) { x + 1.f, y + 1.f, z - 1.f };
							verts[vert_count + 2] = (Vector3) { x + 1.f, y + 1.f, z + 1.f };
							verts[vert_count + 3] = (Vector3) { x + 1.f, y + 1.f, z + 1.f };
							verts[vert_count + 4] = (Vector3) { x - 1.f, y + 1.f, z + 1.f };
							verts[vert_count + 5] = (Vector3) { x - 1.f, y + 1.f, z - 1.f };

							uvs[vert_count]  	= (Vector2) { 0.f, 0.f };
							uvs[vert_count + 1] = (Vector2) { 1.f, 0.f };
							uvs[vert_count + 2] = (Vector2) { 1.f, 1.f };
							uvs[vert_count + 3] = (Vector2) { 1.f, 1.f };
							uvs[vert_count + 4] = (Vector2) { 0.f, 1.f };
							uvs[vert_count + 5] = (Vector2) { 0.f, 0.f };
							vert_count += 6;
						}
					}

					// add mesh down
					j = (z * CHUNK_HEIGHT + (y - 1)) * CHUNK_LENGTH + x;
					if(j < 0 || j > CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT) {
						if(g_chunks[chunk_id].data[j] != 0) {
							verts[vert_count] 	  = (Vector3) { x - 1.f, y - 1.f, z - 1.f };
							verts[vert_count + 1] = (Vector3) { x + 1.f, y - 1.f, z - 1.f };
							verts[vert_count + 2] = (Vector3) { x + 1.f, y - 1.f, z + 1.f };
							verts[vert_count + 3] = (Vector3) { x + 1.f, y - 1.f, z + 1.f };
							verts[vert_count + 4] = (Vector3) { x - 1.f, y - 1.f, z + 1.f };
							verts[vert_count + 5] = (Vector3) { x - 1.f, y - 1.f, z - 1.f };

							uvs[vert_count]  	= (Vector2) { 0.f, 0.f };
							uvs[vert_count + 1] = (Vector2) { 1.f, 0.f };
							uvs[vert_count + 2] = (Vector2) { 1.f, 1.f };
							uvs[vert_count + 3] = (Vector2) { 1.f, 1.f };
							uvs[vert_count + 4] = (Vector2) { 0.f, 1.f };
							uvs[vert_count + 5] = (Vector2) { 0.f, 0.f };
							vert_count += 6;
						}
					}

					// add mesh front
					j = ((z + 1) * CHUNK_HEIGHT + y) * CHUNK_LENGTH + x;
					if(j < 0 || j > CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT) {
						if(g_chunks[chunk_id].data[j] != 0) {
							verts[vert_count] 	  = (Vector3) { x - 1.f, y - 1.f, z + 1.f };
							verts[vert_count + 1] = (Vector3) { x + 1.f, y - 1.f, z + 1.f };
							verts[vert_count + 2] = (Vector3) { x + 1.f, y + 1.f, z + 1.f };
							verts[vert_count + 3] = (Vector3) { x + 1.f, y + 1.f, z + 1.f };
							verts[vert_count + 4] = (Vector3) { x - 1.f, y + 1.f, z + 1.f };
							verts[vert_count + 5] = (Vector3) { x - 1.f, y - 1.f, z + 1.f };

							uvs[vert_count]  	= (Vector2) { 0.f, 0.f };
							uvs[vert_count + 1] = (Vector2) { 1.f, 0.f };
							uvs[vert_count + 2] = (Vector2) { 1.f, 1.f };
							uvs[vert_count + 3] = (Vector2) { 1.f, 1.f };
							uvs[vert_count + 4] = (Vector2) { 0.f, 1.f };
							uvs[vert_count + 5] = (Vector2) { 0.f, 0.f };
							vert_count += 6;
						}
					}

					// add mesh back
					j = ((z - 1) * CHUNK_HEIGHT + y) * CHUNK_LENGTH + x;
					if(j < 0 || j > CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT) {
						if(g_chunks[chunk_id].data[j] != 0) {
							verts[vert_count] 	  = (Vector3) { x - 1.f, y - 1.f, z - 1.f };
							verts[vert_count + 1] = (Vector3) { x + 1.f, y - 1.f, z - 1.f };
							verts[vert_count + 2] = (Vector3) { x + 1.f, y + 1.f, z - 1.f };
							verts[vert_count + 3] = (Vector3) { x + 1.f, y + 1.f, z - 1.f };
							verts[vert_count + 4] = (Vector3) { x - 1.f, y + 1.f, z - 1.f };
							verts[vert_count + 5] = (Vector3) { x - 1.f, y - 1.f, z - 1.f };

							uvs[vert_count]  	= (Vector2) { 0.f, 0.f };
							uvs[vert_count + 1] = (Vector2) { 1.f, 0.f };
							uvs[vert_count + 2] = (Vector2) { 1.f, 1.f };
							uvs[vert_count + 3] = (Vector2) { 1.f, 1.f };
							uvs[vert_count + 4] = (Vector2) { 0.f, 1.f };
							uvs[vert_count + 5] = (Vector2) { 0.f, 0.f };
							vert_count += 6;
						}
					}
				}
			}
		}
	}

	// put them in the memories
	g_chunk_meshes[chunk_id] = Mesh_create(verts, uvs, vert_count);
}

void Chunks_draw() {
	Mesh_bind(g_chunk_meshes[0]);
	glDrawArrays(GL_TRIANGLES, 0, g_chunk_meshes[0].vertex_count);
}
