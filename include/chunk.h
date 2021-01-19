#ifndef CHUNK_H
#define CHUNK_H

#include "mesh.h"
#include "types.h"

#define CHUNK_LENGTH 8 
#define CHUNK_HEIGHT 64
#define CHUNKS_RADIUS 30 
#define CHUNKS_RAYCAST_MAX_STEPS 100

// blocks are stored as ids that can be referenced later on
typedef byte block_id;

// raw chunk mmmmmmmm
typedef struct {
	int x;
	int y;
	block_id data[CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT];
} Chunk;

void Chunks_init();
void Chunks_free();
void Chunks_update();

void Chunk_test(int amount);
void Chunks_setBlock(uint chunk_id, Vector3 block_pos, block_id type);
void Chunks_setBlockRadius(uint chunk_id, Vector3 block_pos, uint radius, block_id type);
int Chunks_getBlockFromRay(Vector3 origin, Vector3 dir, float max_dist, block_id* type, uint* chunk_id, uint* block_id, Vector3* pos);

Vector3 Chunks_getBlockPosFromRay(Vector3 origin, Vector3 dir, float max_dist);
uint Chunks_getBlockIdFromRay(Vector3 origin, Vector3 dir, float max_dist);
uint Chunks_getChunkIdFromRay(Vector3 origin, Vector3 dir, float max_dist);

Vector3 chunk_to_world(Vector3 chunk_pos);
Vector3 world_to_chunk(Vector3 world_pos);

Vector3 block_to_world(Vector3 world_pos, uint chunk_id);
Vector3 world_to_block(Vector3 world_pos);
#endif
