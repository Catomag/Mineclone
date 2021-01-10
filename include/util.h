#ifndef UTIL_H
#define UTIL_H

#include "types.h"

inline float square_root(float number) { 
	int i; 
	float x, y; 
	x = number * 0.5; 
	y = number; 
	i = * (int *) &y; 
	i = 0x5f3759df - (i >> 1); 
	y = * (float *) &i; 
	y = y * (1.5 - (x * y * y)); 
	y = y * (1.5 - (x * y * y)); 
	return number * y; 
}


Buffer load_file(const char* file_name);
uint load_shader(const char* vertex_shader_file, const char* frag_shader_file);
uint load_texture(const char* file_name);


#endif
