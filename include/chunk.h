#ifndef CHUNK_H
#define CHUNK_H

#define CHUNK_LENGTH 16
#define CHUNK_HEIGHT 16

// blocks are stored as ids that can be referenced later on
typedef unsigned char block;

// raw chunk mmmmmmmm
typedef struct {
	int x;
	int y;
	block data[CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT];
} Chunk;



#endif
