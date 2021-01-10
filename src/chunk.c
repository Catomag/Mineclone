#include <stdio.h>
#include <stdlib.h>

#include "../include/glad/glad.h"
#include "../include/chunk.h"


static Chunk* g_chunks;
static Mesh* g_chunk_meshes;
static uint g_chunk_count = CHUNKS_RADIUS * CHUNKS_RADIUS;

static uint shader_program;

void Chunk_generateMesh(uint chunk_id);

void Chunks_init() {
	g_chunk_meshes = (Mesh*) malloc(sizeof(Mesh) * g_chunk_count);
	g_chunks = (Chunk*) malloc(sizeof(Chunk) * g_chunk_count);

	printf("started generating chunks\n");
	for(unsigned int x = 0; x < CHUNKS_RADIUS; x++) {
		for(unsigned int y = 0; y < CHUNKS_RADIUS; y++) {
			unsigned int i = y * CHUNKS_RADIUS + x;

			g_chunks[i].x = x;
			g_chunks[i].y = y;

			byte cap = 3;
			for(unsigned int _x = 0; _x < CHUNK_LENGTH; _x++) {
				for(unsigned int _y = 0; _y < CHUNK_HEIGHT; _y++) {
					for(unsigned int _z = 0; _z < CHUNK_LENGTH; _z++) {
						unsigned int j = (_z * CHUNK_LENGTH + _y) * CHUNK_LENGTH + _x;

						g_chunks[i].data[j] = (_y <= cap);
					}
				}
			}
		}
	}

	for(unsigned int i = 0; i < CHUNKS_RADIUS * CHUNKS_RADIUS; i++)
		Chunk_generateMesh(i);

	printf("finished generating chunks\n");
}


Vector3 neighbour_dirs[6] = {
	{  1.f,  0.f,  0.f },
	{ -1.f,  0.f,  0.f },
	{  0.f,  1.f,  0.f },
	{  0.f, -1.f,  0.f },
	{  0.f,  0.f,  1.f },
	{  0.f,  0.f, -1.f }
};

// this makes the code below simpler
Vector3 quad_vertices[36] = {
	{  .5f,  .5f,  .5f }, // right
	{  .5f,  .5f, -.5f },
	{  .5f, -.5f, -.5f },
	{  .5f, -.5f, -.5f },
	{  .5f, -.5f,  .5f },
	{  .5f,  .5f,  .5f },

	{ -.5f,  .5f,  .5f }, // left
	{ -.5f,  .5f, -.5f },
	{ -.5f, -.5f, -.5f },
	{ -.5f, -.5f, -.5f },
	{ -.5f, -.5f,  .5f },
	{ -.5f,  .5f,  .5f },

	{ -.5f, -.5f, -.5f }, // bottom
	{  .5f, -.5f, -.5f },
	{  .5f, -.5f,  .5f },
	{  .5f, -.5f,  .5f },
	{ -.5f, -.5f,  .5f },
	{ -.5f, -.5f, -.5f },

	{ -.5f,  .5f, -.5f }, // top
	{  .5f,  .5f, -.5f },
	{  .5f,  .5f,  .5f },
	{  .5f,  .5f,  .5f },
	{ -.5f,  .5f,  .5f },
	{ -.5f,  .5f, -.5f },

	{ -.5f, -.5f,  .5f }, // front
	{  .5f, -.5f,  .5f },
	{  .5f,  .5f,  .5f },
	{  .5f,  .5f,  .5f },
	{ -.5f,  .5f,  .5f },
	{ -.5f, -.5f,  .5f },

	{ -.5f, -.5f, -.5f }, // back
	{  .5f, -.5f, -.5f },
	{  .5f,  .5f, -.5f },
	{  .5f,  .5f, -.5f },
	{ -.5f,  .5f, -.5f },
	{ -.5f, -.5f, -.5f }
};

Vector2 quad_uvs[12] = {
	{ 1.f, 0.f }, // left and right uvs
	{ 1.f, 1.f },
	{ 0.f, 1.f },
	{ 0.f, 1.f },
	{ 0.f, 0.f },
	{ 1.f, 0.f },

	{ 0.f, 1.f }, // top, bottom, back and front uvs
	{ 1.f, 1.f },
	{ 1.f, 0.f },
	{ 1.f, 0.f },
	{ 0.f, 0.f },
	{ 0.f, 1.f },
};

void Chunk_generateMesh(uint chunk_id) {

	unsigned int quad_count = 0;
	for(short x = 0; x < CHUNK_LENGTH; x++) {
		for(short y = 0; y < CHUNK_HEIGHT; y++) {
			for(short z = 0; z < CHUNK_LENGTH; z++) {
				unsigned int i = (z * CHUNK_LENGTH + y) * CHUNK_LENGTH + x;
				quad_count += 6 * (g_chunks[chunk_id].data[i] != 0);
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
				unsigned int block_index = (z * CHUNK_LENGTH + y) * CHUNK_LENGTH + x;
				if(g_chunks[chunk_id].data[block_index] != 0) {
					// loop through neighbours
					for(byte i = 0; i < 6; i++) {
						unsigned int neighbour_index;
						block_id neighbour;

						if(y + neighbour_dirs[i].y < 0 || y + neighbour_dirs[i].y >= CHUNK_HEIGHT) {
							neighbour = 0;
						}
						else if((x + neighbour_dirs[i].x < 0 || x + neighbour_dirs[i].x >= CHUNK_LENGTH) || 
								(z + neighbour_dirs[i].z < 0 || z + neighbour_dirs[i].z >= CHUNK_LENGTH)) {
							Vector2 chunk_neighbour;
							chunk_neighbour.x = g_chunks[chunk_id].x + neighbour_dirs[i].x;
							chunk_neighbour.x += (chunk_neighbour.x < 0) * CHUNKS_RADIUS;
							chunk_neighbour.x -= (chunk_neighbour.x == CHUNKS_RADIUS) * CHUNKS_RADIUS;

							chunk_neighbour.y = g_chunks[chunk_id].y + neighbour_dirs[i].y;
							chunk_neighbour.y += (chunk_neighbour.x < 0) * CHUNKS_RADIUS;
							chunk_neighbour.y -= (chunk_neighbour.x == CHUNKS_RADIUS) * CHUNKS_RADIUS;

							uint chunk_neighbour_id = chunk_neighbour.y * CHUNKS_RADIUS + chunk_neighbour.x;

							Vector3 neighbour_pos;
							neighbour_pos.x = x + neighbour_dirs[i].x;
							neighbour_pos.x += (neighbour_pos.x < 0) * CHUNK_LENGTH;
							neighbour_pos.x -= (neighbour_pos.x == CHUNK_LENGTH) * CHUNK_LENGTH;

							neighbour_pos.y = y + neighbour_dirs[i].y;

							neighbour_pos.z = z + neighbour_dirs[i].z;
							neighbour_pos.z += (neighbour_pos.z < 0) * CHUNK_LENGTH;
							neighbour_pos.z -= (neighbour_pos.z == CHUNK_LENGTH) * CHUNK_LENGTH;

							neighbour_index = (neighbour_pos.z * CHUNK_LENGTH + neighbour_pos.y) * CHUNK_LENGTH + neighbour_pos.x;
							neighbour = g_chunks[chunk_neighbour_id].data[neighbour_index];
						}
						else {
							neighbour_index = ((neighbour_dirs[i].z + z) * CHUNK_LENGTH + (neighbour_dirs[i].y + y)) * CHUNK_LENGTH + (neighbour_dirs[i].x + x);
							neighbour = g_chunks[chunk_id].data[neighbour_index];
						}

						// loop through vertices
						for(byte j = 0; j < 6; j++) {
							if(neighbour == 0) {
								verts[vert_count] = (Vector3) { x + quad_vertices[(i * 6) + j].x, -y + quad_vertices[(i * 6) + j].y, z + quad_vertices[(i * 6) + j].z };

								uvs[vert_count]	= (Vector2) { quad_uvs[(6 * (i > 1)) + j].x, quad_uvs[(6 * (i > 1)) + j].y };
								vert_count++;
							}
						}
					}
				}
			}
		}
	}


	// put them in the memories
	g_chunk_meshes[chunk_id] = Mesh_create(verts, uvs, vert_count);
}

void Chunk_test(int amount) {
	for(unsigned int i = 0; i < CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT; i++)
		g_chunks[0].data[i] = 0;

	for(unsigned int i = amount; i < CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT; i++)
		g_chunks[0].data[i] = 1;
//		g_chunks[0].data[i] = ((rand() % (CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT)) >= amount);

	Mesh_destroy(&g_chunk_meshes[0]);
	Chunk_generateMesh(0);
}

void Chunks_draw(uint shader_program) {
	Matrix transform;

	for(unsigned int i = 0; i < g_chunk_count; i++) {
		transform = MatrixTranslate(g_chunks[i].x * CHUNK_LENGTH, 0, g_chunks[i].y * CHUNK_LENGTH);

		glUniformMatrix4fv(glGetUniformLocation(shader_program, "transform"), 1, GL_FALSE, MatrixToFloatV(transform).v);
		Mesh_bind(g_chunk_meshes[i]);
		glDrawArrays(GL_TRIANGLES, 0, g_chunk_meshes[i].vertex_count);
	}

	for(unsigned int i = 0; i < g_chunk_count; i++) {
		transform = MatrixMultiply(MatrixRotateX(PI), MatrixTranslate(g_chunks[i].x * CHUNK_LENGTH, -CHUNK_HEIGHT * 2, g_chunks[i].y * CHUNK_LENGTH));

		glUniformMatrix4fv(glGetUniformLocation(shader_program, "transform"), 1, GL_FALSE, MatrixToFloatV(transform).v);
		Mesh_bind(g_chunk_meshes[i]);
		glDrawArrays(GL_TRIANGLES, 0, g_chunk_meshes[i].vertex_count);
	}
}
