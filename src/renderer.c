#include <stdlib.h>
#include <stdio.h>

#define RAYMATH_STANDALONE
#include <raymath.h>

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../include/renderer.h"
#include "../include/chunks_internal.h"
#include "../include/util.h"

static GLFWwindow* window;
static Camera* camera;

static uint block_shader;
static uint debug_chunk_shader;
static uint texture;


void framebuffer_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	Camera_set(camera, PI / 2.f, width, height, 0.0001f, 3000.f);
}


// hacks

int get_key(int key) {
	return glfwGetKey(window, key);
}

void get_cursor(double* x, double* y) {
	glfwGetCursorPos(window, x, y);
}

void set_input_mode(int input, int mode) {
	glfwSetInputMode(window, input, mode);
}

int Renderer_shouldClose() {
	return glfwWindowShouldClose(window);
}


void Renderer_init(Camera* cam, int width, int height) {
	camera = cam;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "testapp", NULL, NULL);
	if(window == NULL) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed starting GLAD\n");
		exit(-1);
	}

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_callback);

	glEnable(GL_DEPTH_TEST);

	block_shader = load_shader("assets/vert.glsl", "assets/frag.glsl");
	debug_chunk_shader = load_shader("assets/vert.glsl", "assets/debug_chunk_frag.glsl");
	texture = load_texture("assets/thing.png");
}

void Renderer_display() {
	// draw chunks

	Matrix transform = MatrixIdentity();
	glUseProgram(block_shader);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniformMatrix4fv(glGetUniformLocation(block_shader, "transform"), 1, GL_FALSE, MatrixToFloatV(transform).v);
	glUniformMatrix4fv(glGetUniformLocation(block_shader, "view"), 1, GL_FALSE, MatrixToFloatV(camera->view).v);
	glUniformMatrix4fv(glGetUniformLocation(block_shader, "projection"), 1, GL_FALSE, MatrixToFloatV(camera->projection).v);
	glUniform3f(glGetUniformLocation(block_shader, "cam_pos"), camera->pos.x, camera->pos.y, camera->pos.z);

	for(unsigned int i = 0; i < CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT; i++) {
		transform = MatrixTranslate(chunks[i].x * CHUNK_LENGTH, 0, chunks[i].y * CHUNK_LENGTH);

		glUniformMatrix4fv(glGetUniformLocation(block_shader, "transform"), 1, GL_FALSE, MatrixToFloatV(transform).v);
		Mesh_bind(chunk_meshes[i]);
		glDrawArrays(GL_TRIANGLES, 0, chunk_meshes[i].vertex_count);
	}

//	glUseProgram(debug_chunk_shader);
//	Mesh_bind(chunk_debug_mesh);
//
//	glUniformMatrix4fv(glGetUniformLocation(debug_chunk_shader, "transform"), 1, GL_FALSE, MatrixToFloatV(transform).v);
//	glUniformMatrix4fv(glGetUniformLocation(debug_chunk_shader, "view"), 1, GL_FALSE, MatrixToFloatV(camera->view).v);
//	glUniformMatrix4fv(glGetUniformLocation(debug_chunk_shader, "projection"), 1, GL_FALSE, MatrixToFloatV(camera->projection).v);
//	glUniform3f(glGetUniformLocation(debug_chunk_shader, "cam_pos"), camera->pos.x, camera->pos.y, camera->pos.z);
//
//	for(unsigned int i = 0; i < CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT; i++) {
//		transform = MatrixTranslate(chunks[i].x * CHUNK_LENGTH + (CHUNK_LENGTH / 2.f) - .5f, -(CHUNK_HEIGHT / 2.f) + .5f, chunks[i].y * CHUNK_LENGTH + (CHUNK_LENGTH / 2.f) - .5f);
//
//		glUniformMatrix4fv(glGetUniformLocation(debug_chunk_shader, "transform"), 1, GL_FALSE, MatrixToFloatV(transform).v);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//	}
}

void Renderer_clear() {
	glfwSwapBuffers(window);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer_free() {
	for(unsigned int i = 0; i < CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT; i++) {
		Mesh_destroy(&chunk_meshes[i]);
	}
}
