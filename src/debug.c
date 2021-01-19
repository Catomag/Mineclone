#define _DEBUG_INTERNAL_
#include "../include/debug.h"
#include "../include/types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULL_SIZE (ulong)-1

typedef enum {
	MALLOC,
	FREE,
	REALLOC
} Allocation_Type;

typedef struct {
	Allocation_Type type; // 4 bytes
	uint line; // 4 bytes
	ulong address; // 8 bytes
	ulong size; // 8 bytes
	char* file_name; // ? bytes
} Allocation;



Allocation* g_debug_log;
ulong g_debug_log_length = 0;
ulong g_debug_log_allocated = 0;

ulong g_allocations = 0;
ulong g_allocated_bytes = 0;

ulong g_frees = 0;
ulong g_freed_bytes = 0;



void record(Allocation allocation) {
	// if there are no allocated bytes, allocate some memory
	if(g_debug_log_allocated == 0) {
		g_debug_log_allocated = 4;
		g_debug_log = (Allocation*) malloc(sizeof(Allocation) * 4);
	}

	// when g_debug_log runs out of memory allocate some more
	if(g_debug_log_allocated <= g_debug_log_length) {
		g_debug_log_allocated *= 2;
		g_debug_log = (Allocation*) realloc(g_debug_log, g_debug_log_allocated * sizeof(Allocation));
	}

#ifdef _DEBUG_PRINT_ASAP_
	switch(allocation.type) {
		case MALLOC:
			printf("\033[33m");
			printf("Allocated: ");
			break;
		case REALLOC:
			printf("\033[33m");
			printf("Rellocated: ");
			break;
		case FREE:
			printf("\033[32m");
			printf("Freed: ");
	}

	printf("\033[0m");
	printf("%lu bytes, line: %u, file: %s, address: %X\n", allocation.size, allocation.line, allocation.file_name, allocation.address);
#endif

	g_debug_log[g_debug_log_length] = allocation;
	g_debug_log_length++;
}



void* debug_malloc(ulong size, uint line, char *file_name) {
	void *res = malloc(size);
	Allocation allocation;

	allocation.address = (ulong)res;
	allocation.size = size;
	allocation.type = MALLOC;

	allocation.line = line;
	allocation.file_name = file_name;

	record(allocation);

	g_allocations++;
	g_allocated_bytes += size;

	return res;
}

void* debug_realloc(void *ptr, ulong size, uint line, char *file_name) {
	for(ulong i = 0; i < g_debug_log_length; i++) {
		if(g_debug_log[i].address == (ulong)ptr) {
			g_allocated_bytes += g_debug_log[i].size - size;
			break;
		}
	}

	void *res = realloc(ptr, size);
	
	Allocation allocation;

	allocation.address = (ulong)res;
	allocation.size = size;
	allocation.type = REALLOC;

	allocation.line = line;
	allocation.file_name = file_name;

	record(allocation);

	// not sure about this, need to do some testing
	/*if(ptr != res)
		g_allocations++;*/
	return res;
}

void debug_free(void *ptr, uint line, char* file_name) {
	ulong size = NULL_SIZE;

	// cycles through the allocations from most recent to least recent, if the first allocation in that address is a free then it is invalid, else it is a valid free
	for(ulong i = g_debug_log_length; i != -1u; i--) {
		if(g_debug_log[i].address == (ulong)ptr) {
			if(g_debug_log[i].type == FREE) {
				size = NULL_SIZE;
				break;
			}

			else {
				size = g_debug_log[i].size;
				break;
			}
		}
	}

	// detects invalid frees and ELIMINATES THEM!!!
	if(size == NULL_SIZE) {
		printf("Error, invalid free. Line: %u, file: %s\n", line, file_name);
		exit(-1);
	}

	Allocation allocation;

	allocation.address = (ulong)ptr;
	allocation.size = size;
	allocation.type = FREE;

	allocation.line = line;
	allocation.file_name = file_name;

	g_frees++;
	g_freed_bytes += size;
	record(allocation);

	free(ptr);
}


ulong debug_memory_allocated() {
	return g_allocated_bytes - g_freed_bytes;
}

ulong debug_total_memory_allocated() {
	return g_allocated_bytes;
}


void debug_log() {
		printf("TYPE	ADDRESS		BYTES	LINE	FILE\n");
	// cycle through every allocation and print their information
	for(ulong i = 0; i < g_debug_log_length; i++) {
		if(g_debug_log[i].type == MALLOC) {
			// sets cool terminal colours, on unix and mac (windows is lame and doesn't support ansi escape codes)
#ifdef __unix__
			printf("\033[33m");
#endif
			printf("Malloc	");

		}

		else if(g_debug_log[i].type == REALLOC) {
#ifdef __unix__
			printf("\033[33m");
#endif
			printf("Realloc	");
		}

		else {
#ifdef __unix__
			printf("\033[32m");
#endif
			printf("Free	");
		}

	// sets the colour back to white
#ifdef __unix__
			printf("\033[0m");
#endif

		printf("%lx", g_debug_log[i].address);
		printf("	%lu", g_debug_log[i].size);
		printf("	%u", g_debug_log[i].line);
		printf("	%s\n", g_debug_log[i].file_name);
	}


	// print all this useful debug info

	printf("%lu allocations/", g_allocations);
	printf("%lu frees\n", g_frees);

	printf("%lu currently allocated bytes\n", g_allocated_bytes - g_freed_bytes);
	printf("%lu total allocated bytes\n", g_allocated_bytes);
	printf("%lu freed bytes\n", g_freed_bytes);
#ifdef __unix__
		printf("\033[0m");
#endif
}

void debug_log_totals() {
	printf("%lu allocations/", g_allocations);
	printf("%lu frees\n", g_frees);

	printf("%lu currently allocated bytes\n", g_allocated_bytes - g_freed_bytes);
	printf("%lu total allocated bytes\n", g_allocated_bytes);
	printf("%lu freed bytes\n", g_freed_bytes);
}	

void debug_log_empty() {
	if(g_debug_log_allocated != 0) {
		free(g_debug_log);
		g_debug_log_length = 0;
		g_debug_log_allocated = 0;
	}
}

void debug_error_internal(char* message, uint line, char* file_name) {
	// once again unix escape codes are epic
#ifdef __unix__
	printf("\033[31m");
#endif
	printf("Error: %s Line: %u, File: %s", message, line, file_name);
	exit(-1);
}
