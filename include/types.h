#ifndef TYPES_H
#define TYPES_H

typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char byte;

typedef struct {
	float x;
	float y;
	float z;
} Vec3;

typedef struct {
	float x;
	float y;
} Vec2;

typedef struct {
	byte* data;
	ulong size;
} Buffer;

#endif
