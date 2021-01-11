#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"

int get_key(int key);
void get_cursor(double* mouse_x, double* mouse_y);
void set_input_mode(int input, int mode);

void Renderer_init(Camera* camera, int width, int height);
void Renderer_free();

void Renderer_display();
int Renderer_shouldClose();
void Renderer_clear();

#endif
