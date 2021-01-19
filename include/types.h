#ifndef TYPES_H
#define TYPES_H

typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char byte;


typedef struct {
	byte* data;
	ulong size;
} Buffer;

#endif
