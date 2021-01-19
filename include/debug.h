#pragma once
#include "types.h"

// Alternatives to normal allocation and deallocation functions which store the line and filename for each allocation
void* debug_malloc(ulong size, uint line, char* file_name);
void* debug_realloc(void *ptr, ulong size, uint line, char* file_name);
void debug_free(void *ptr, uint line, char* file);

void debug_error_internal(char* message, uint line, char* file_name); // prints the error message with line number, filename and epic red colour (if it's in superior unix terminal)

#define debug_error(message) debug_error_internal(message, __LINE__, __FILE__)
#define debug_thisran() debug_error_internal("This ran", __LINE__, __FILE__)

void debug_log(); // Prints all allocations and deallocations
void debug_log_totals();
void debug_log_empty(); // Clears debug log

ulong debug_memory_allocated(); // returns the amount of memory that is currently being used by the program in bytes
ulong debug_total_memory_allocated(); // returns the total amount of memory that has been used by the program

// If debug mode is defined replace all malloc and free functions with their debug versions to log EVERY SINGLE memory allocation
#ifdef _DEBUG_
	// don't do this on the debug.c file cause it could break some things...
	#ifndef _DEBUG_INTERNAL_
		#define malloc(x) debug_malloc(x, __LINE__, __FILE__)
		#define free(x) debug_free(x, __LINE__, __FILE__)
		#define realloc(x, y) debug_realloc(x, y, __LINE__, __FILE__)
	#endif
#endif
