#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "../include/glad/glad.h"
#include "../include/chunks_internal.h"
#include "../include/util.h"
#include "../include/debug.h"


Chunk* chunks;
Mesh* chunk_meshes;
byte* chunk_changed;
                         
Mesh chunk_debug_mesh;


void Chunk_generateMesh(unsigned int chunk_id);


Vector3 chunk_to_world(Vector3 chunk_coords) {
	Vector3 res;
	res.x = chunk_coords.x * CHUNK_LENGTH;
	res.y = chunk_coords.y * CHUNK_HEIGHT;
	res.z = chunk_coords.z * CHUNK_LENGTH;

	return res;
}

Vector3 world_to_chunk(Vector3 world_coords) {
	Vector3 res;
	res.x = ((int)world_coords.x / (int) CHUNK_LENGTH);
	res.y = ((int)world_coords.y / (int) CHUNK_HEIGHT);
	res.z = ((int)world_coords.z / (int) CHUNK_LENGTH);

	return res;
}

Vector3 world_to_block(Vector3 world_coords) {
	Vector3 res;
	res.x = ((int) world_coords.x) % CHUNK_LENGTH;
	res.y = ((int) world_coords.y);
	res.z = ((int) world_coords.z) % CHUNK_LENGTH;

	return res;
}

Vector3 block_to_world(Vector3 block_coords, uint chunk_id) {
	Vector3 res;
	res.x = block_coords.x + (chunks[chunk_id].x * CHUNK_LENGTH);
	res.y = block_coords.y;
	res.z = block_coords.z + (chunks[chunk_id].y * CHUNK_LENGTH);

	return res;
}

void Chunks_init() {
	unsigned int chunk_count = CHUNK_HEIGHT * CHUNK_LENGTH * CHUNK_LENGTH;
	chunk_meshes = (Mesh*) malloc(sizeof(Mesh) * chunk_count);
	chunks = (Chunk*) malloc(sizeof(Chunk) * chunk_count);
	chunk_changed = (byte*) malloc(sizeof(byte) * chunk_count);

	printf("started generating chunks\n");
	for(unsigned int x = 0; x < CHUNKS_RADIUS; x++) {
		for(unsigned int y = 0; y < CHUNKS_RADIUS; y++) {
			unsigned int i = y * CHUNKS_RADIUS + x;

			chunks[i].x = x;
			chunks[i].y = y;
			chunk_changed[i] = 0;

			for(unsigned int index = 0; index < CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT; index++) {
				chunks[i].data[index] = 0;//(_y <= cap);
			}

			unsigned int cap = ((rand() % 2) == 0) * 32 + 1;
			for(unsigned int _x = 0; _x < CHUNK_LENGTH; _x++) {
				for(unsigned int _y = 0; _y < CHUNK_HEIGHT; _y++) {
					for(unsigned int _z = 0; _z < CHUNK_LENGTH; _z++) {
						unsigned int j = (_z * CHUNK_HEIGHT + _y) * CHUNK_LENGTH + _x;

						chunks[i].data[j] = 1;// && (_z > 0 && _z < CHUNK_LENGTH) && (_x > 0 && _x < CHUNK_LENGTH);
					}
				}
			}
		}
	}

	printf("generating chunk meshes\n");
	for(unsigned int i = 0; i < CHUNKS_RADIUS * CHUNKS_RADIUS; i++) {
		chunk_meshes[i] = Mesh_create(quad_vertices, quad_uvs, 36);
		Chunk_generateMesh(i);
	}

	Vector3 vertices[36];
	Vector2 uvs[36];

	for(byte i = 0; i < 36; i++) {
		vertices[i].x = quad_vertices[i].x * CHUNK_LENGTH;
		vertices[i].y = quad_vertices[i].y * CHUNK_HEIGHT;
		vertices[i].z = quad_vertices[i].z * CHUNK_LENGTH;

		uvs[i] = quad_uvs[i];
	}

	chunk_debug_mesh = Mesh_create(vertices, uvs, 36);
	printf("finished generating chunks\n");
}



void Chunk_generateMesh(uint chunk_id) {
	unsigned int quad_count = 0;
	for(short x = 0; x < CHUNK_LENGTH; x++) {
		for(short y = 0; y < CHUNK_HEIGHT; y++) {
			for(short z = 0; z < CHUNK_LENGTH; z++) {
				unsigned int i = (z * CHUNK_HEIGHT + y) * CHUNK_LENGTH + x;
				quad_count += 6 * (chunks[chunk_id].data[i] != 0);
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
				unsigned int block_index = (z * CHUNK_HEIGHT + y) * CHUNK_LENGTH + x;
				if(chunks[chunk_id].data[block_index] != 0) {
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
							chunk_neighbour.x = chunks[chunk_id].x + neighbour_dirs[i].x;
							chunk_neighbour.x += (chunk_neighbour.x < 0) * CHUNKS_RADIUS;
							chunk_neighbour.x -= (chunk_neighbour.x == CHUNKS_RADIUS) * CHUNKS_RADIUS;

							chunk_neighbour.y = chunks[chunk_id].y + neighbour_dirs[i].z;
							chunk_neighbour.y += (chunk_neighbour.y < 0) * CHUNKS_RADIUS;
							chunk_neighbour.y -= (chunk_neighbour.y == CHUNKS_RADIUS) * CHUNKS_RADIUS;

							uint chunk_neighbour_id = chunk_neighbour.y * CHUNKS_RADIUS + chunk_neighbour.x;

							Vector3 neighbour_pos;
							neighbour_pos.x = x - neighbour_dirs[i].x;
							neighbour_pos.x += (neighbour_pos.x < 0) * CHUNK_LENGTH;
							neighbour_pos.x -= (neighbour_pos.x == CHUNK_LENGTH) * CHUNK_LENGTH;

							neighbour_pos.y = y + neighbour_dirs[i].y;

							neighbour_pos.z = z + neighbour_dirs[i].z;
							neighbour_pos.z += (neighbour_pos.z < 0) * CHUNK_LENGTH;
							neighbour_pos.z -= (neighbour_pos.z == CHUNK_LENGTH) * CHUNK_LENGTH;

							neighbour_index = (neighbour_pos.z * CHUNK_HEIGHT + neighbour_pos.y) * CHUNK_LENGTH + neighbour_pos.x;
							neighbour = chunks[chunk_neighbour_id].data[neighbour_index];
						}
						else {
							neighbour_index = ((neighbour_dirs[i].z + z) * CHUNK_HEIGHT + (neighbour_dirs[i].y + y)) * CHUNK_LENGTH + (neighbour_dirs[i].x + x);
							neighbour = chunks[chunk_id].data[neighbour_index];
						}

						// loop through vertices
						for(byte j = 0; j < 6; j++) {
							if(neighbour == 0) {
								verts[vert_count] = (Vector3) { x + quad_vertices[(i * 6) + j].x, y + quad_vertices[(i * 6) + j].y, z + quad_vertices[(i * 6) + j].z };

								uvs[vert_count]	= (Vector2) { quad_uvs[(i * 6) + j].x, quad_uvs[(i * 6) + j].y };
								vert_count++;
							}
						}
					}
				}
			}
		}

		chunk_changed[chunk_id] = 0;
	}

	// put them in the memories
	Mesh_update(&chunk_meshes[chunk_id], verts, uvs, vert_count);
}

void Chunks_setBlock(uint chunk_id, Vector3 block_pos, block_id type) {
	assert(chunk_id >= 0 && chunk_id < CHUNKS_RADIUS * CHUNKS_RADIUS);

	if( block_pos.x >= 0 && block_pos.x < CHUNK_LENGTH &&
		block_pos.y >= 0 && block_pos.y < CHUNK_HEIGHT &&
		block_pos.z >= 0 && block_pos.z < CHUNK_LENGTH) {
		uint index = (block_pos.z * CHUNK_HEIGHT + block_pos.y) * CHUNK_LENGTH + block_pos.x;
		chunk_changed[chunk_id] = (type != chunks[chunk_id].data[index]);
		chunks[chunk_id].data[index] = type;
	printf("block set!\n");
	}
}

void Chunks_update() {
	for(unsigned int i = 0; i < CHUNKS_RADIUS * CHUNKS_RADIUS; i++) {
		if(chunk_changed[i] == 1) {
			Chunk_generateMesh(i);
		}
	}
}

// TODO: revisit this code if world wrapping is funky
int Chunks_getBlockFromRay(Vector3 origin, Vector3 dir, float max_dist, block_id* type, uint* chunk_id, uint* block_id, Vector3* pos) {
	Vector3 current_pos = origin;
	float step = (max_dist / CHUNKS_RAYCAST_MAX_STEPS);

//	printf("Start pos: %f, %f, %f\n", current_pos.x, current_pos.y, current_pos.z);
//	printf("Step: %f\n", step);
//	printf("Max steps: %i\n", CHUNKS_RAYCAST_MAX_STEPS);

	// slowly step through possible positions
	for(uint i = 0; i < CHUNKS_RAYCAST_MAX_STEPS; i++) {
//		printf("Iteration: %i\n", i);
		current_pos.x += dir.x * step;
		current_pos.y += dir.y * step;
		current_pos.z += dir.z * step;


		// check if chunk exists
		Vector3 chunk_pos = world_to_chunk(current_pos);
		Vector3 block_pos = world_to_block(current_pos);

		if((chunk_pos.x >= 0 && chunk_pos.z >= 0) && (chunk_pos.x < CHUNKS_RADIUS && chunk_pos.z < CHUNKS_RADIUS) && (current_pos.y <= CHUNK_HEIGHT)) {
			uint chunk_index = CHUNKS_RADIUS * chunk_pos.z + chunk_pos.x;
			uint block_index = (block_pos.z * CHUNK_HEIGHT + block_pos.y) * CHUNK_LENGTH + block_pos.x;
//			printf("Found valid chunk!\n");
			printf("Chunk coords: %f, %f, %f\n", chunk_pos.x, chunk_pos.y, chunk_pos.z);
			printf("Current pos: %f, %f, %f\n", current_pos.x, current_pos.y, current_pos.z);
			printf("Block coords: %f, %f, %f\n", block_pos.x, block_pos.y, block_pos.z);
			printf("Block id: %i\n", block_index);

			if(chunks[chunk_index].data[block_index] > 0) {
				if(chunk_id != NULL)
					*chunk_id = chunk_index;
				if(block_id != NULL)
					*block_id = block_index;
				if(type != NULL)
					*type = chunks[chunk_index].data[block_index];
				if(pos != NULL)
					*pos = current_pos;

				return 1;
			}
		}
	}

	return 0;
}

Vector3 Chunks_getBlockPosFromRay(Vector3 origin, Vector3 dir, float max_dist) {
	Vector3 res;
	Chunks_getBlockFromRay(origin, dir, max_dist, NULL, NULL, NULL, &res);
	return res;
}

uint Chunks_getBlockIdFromRay(Vector3 origin, Vector3 dir, float max_dist) {
	uint res;
	Chunks_getBlockFromRay(origin, dir, max_dist, NULL, NULL, &res, NULL);
	return res;
}

uint Chunks_getChunkIdFromRay(Vector3 origin, Vector3 dir, float max_dist) {
	uint res;
	Chunks_getBlockFromRay(origin, dir, max_dist, NULL, &res, NULL, NULL);
	return res;
}

void Chunks_free() {
	// destroy all meshes
	Mesh_destroy(&chunk_debug_mesh);
	for(uint i = 0; i < CHUNKS_RADIUS * CHUNKS_RADIUS; i++)
		Mesh_destroy(&chunk_meshes[i]);
	
	// destroy all chunk data
	free(chunks);
	free(chunk_meshes);
}

void Chunk_test(int amount) {
	for(unsigned int i = 0; i < CHUNKS_RADIUS * CHUNKS_RADIUS; i++) {
		for(unsigned int j = 0; j < CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT; j++)
			chunks[i].data[j] = 0;

		for(unsigned int j = amount; j < CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT; j++)
			chunks[i].data[j] = 1;
//		chunks[0].data[i] = ((rand() % (CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT)) >= amount);
		Mesh_destroy(&chunk_meshes[i]);
		Chunk_generateMesh(i);
	}
}
