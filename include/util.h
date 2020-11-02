#ifndef UTIL_H
#define UTIL_H

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


#endif
