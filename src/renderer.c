#include <stdlib.h>
#include <stdio.h>

#define RAYMATH_STANDALONE
#include <raymath.h>

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../include/renderer.h"
#include "../include/chunks_internal.h"
#include "../include/util.h"
#include "../include/debug.h"

static GLFWwindow* window;
static Camera* camera;

static Mesh cube;

static uint line_vao;
static uint line_vbo;

static uint block_shader;
static uint debug_chunk_shader;
static uint white_shader;
static uint texture;


void framebuffer_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	Camera_set(camera, PI / 2.f, width, height, 0.0001f, 3000.f);
}


// hacks

int get_mouse_button(int button) {
	return glfwGetMouseButton(window, button);
}

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

	printf("starting glfw\n");
	if(!glfwInit()) {
		debug_error("GLFW init failed!\n");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	printf("making window\n");
	window = glfwCreateWindow(width, height, "testapp", NULL, NULL);
	glfwSwapInterval(1);

	printf("checking if window is null\n");
	if(window == 0) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		exit(-1);
	}
	printf("making window context\n");
	glfwMakeContextCurrent(window);

	printf("starting glad\n");
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed starting GLAD\n");
		exit(-1);
	}

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_callback);

	glEnable(GL_DEPTH_TEST);

	printf("loading shaders\n");
	block_shader = load_shader("assets/vert.glsl", "assets/frag.glsl");
	debug_chunk_shader = load_shader("assets/vert.glsl", "assets/debug_chunk_frag.glsl");
	white_shader = load_shader("assets/vert.glsl", "assets/line_frag.glsl");

	printf("loading texture\n");
	texture = load_texture("assets/rock.png");

	cube = Mesh_create(quad_vertices, quad_uvs, 36);

	glGenVertexArrays(1, &line_vao);
	glBindVertexArray(line_vao);

	glGenBuffers(1, &line_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, line_vbo);

	Vector3 line[] = {
		{ 0.f, 0.f, 0.f },
		{ 0.f, .25f, 0.f }
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * 2, line, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*) 0);
	glEnableVertexAttribArray(0);
}

void Renderer_display() {
	// draw chunks

	Vector3 chunk_pos;
	chunk_pos = world_to_chunk(camera->pos);

//	printf("cam pos: %f, %f, %f\n", camera->pos.x, camera->pos.y, camera->pos.z);
//	printf("cam pos: %f, %f, %f\n", chunk_pos.x, chunk_pos.y, chunk_pos.z);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	Matrix transform = MatrixIdentity();
	glUseProgram(block_shader);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniformMatrix4fv(glGetUniformLocation(block_shader, "transform"), 1, GL_FALSE, MatrixToFloatV(transform).v);
	glUniformMatrix4fv(glGetUniformLocation(block_shader, "view"), 1, GL_FALSE, MatrixToFloatV(camera->view).v);
	glUniformMatrix4fv(glGetUniformLocation(block_shader, "projection"), 1, GL_FALSE, MatrixToFloatV(camera->projection).v);
	glUniform3f(glGetUniformLocation(block_shader, "cam_pos"), camera->pos.x, camera->pos.y, camera->pos.z);
	glUniform1f(glGetUniformLocation(block_shader, "testing"), 0);

	for(unsigned int x = 0; x < CHUNKS_RADIUS; x++) {
		for(unsigned int y = 0; y < CHUNKS_RADIUS; y++) {
			int chunk_x = (x + ((int) chunk_pos.x)) % CHUNKS_RADIUS;
			int chunk_y = (y + ((int) chunk_pos.z)) % CHUNKS_RADIUS;
			int i = chunk_y * CHUNKS_RADIUS + chunk_x;
			if(i >= 900)
				printf("%i\n", i);

			transform = MatrixTranslate((chunks[i].x) * CHUNK_LENGTH, 0, (chunks[i].y) * CHUNK_LENGTH);

			glUniformMatrix4fv(glGetUniformLocation(block_shader, "transform"), 1, GL_FALSE, MatrixToFloatV(transform).v);
			Mesh_bind(chunk_meshes[i]);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_TRIANGLES, 0, chunk_meshes[i].vertex_count);
		}
	}

//	// TODO: test if changing transform multiplication order in shader makes thingy work
//	for(unsigned int i = 0; i < CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT; i++) {
//		transform = MatrixMultiply(MatrixScale(1, -1, 1), MatrixTranslate(chunks[i].x * CHUNK_LENGTH, -CHUNK_HEIGHT * 2, chunks[i].y * CHUNK_LENGTH));
//
//		glUniformMatrix4fv(glGetUniformLocation(block_shader, "transform"), 1, GL_FALSE, MatrixToFloatV(transform).v);
//		Mesh_bind(chunk_meshes[i]);
//		glDrawArrays(GL_TRIANGLES, 0, chunk_meshes[i].vertex_count);
//	}

	// draw selected cube
	Vector3 selected_pos;

	selected_pos = Chunks_getBlockPosFromRay((Vector3) { camera->pos.x + .5f, camera->pos.y + .5f, camera->pos.z + .5f }, camera->front, 10.f);

	Mesh_bind(cube);
	glUseProgram(debug_chunk_shader);
	glUniformMatrix4fv(glGetUniformLocation(debug_chunk_shader, "view"), 1, GL_FALSE, MatrixToFloatV(camera->view).v);
	glUniformMatrix4fv(glGetUniformLocation(debug_chunk_shader, "projection"), 1, GL_FALSE, MatrixToFloatV(camera->projection).v);
	glUniform3f(glGetUniformLocation(debug_chunk_shader, "cam_pos"), camera->pos.x, camera->pos.y, camera->pos.z);

	transform = MatrixMultiply(MatrixScale(1.05f, 1.05f, 1.05f), MatrixTranslate((int) selected_pos.x, (int) selected_pos.y, (int) selected_pos.z));
	glUniformMatrix4fv(glGetUniformLocation(debug_chunk_shader, "transform"), 1, GL_FALSE, MatrixToFloatV(transform).v);
	glUniform1f(glGetUniformLocation(debug_chunk_shader, "testing"), 1);
	glDrawArrays(GL_TRIANGLES, 0, chunk_debug_mesh.vertex_count);


	selected_pos = Chunks_getBlockPosFromRay((Vector3) { camera->pos.x , camera->pos.y , camera->pos.z }, camera->front, 10.f);
	glBindVertexArray(line_vao);
	glUseProgram(white_shader);
	glUniformMatrix4fv(glGetUniformLocation(white_shader, "view"), 1, GL_FALSE, MatrixToFloatV(camera->view).v);
	glUniformMatrix4fv(glGetUniformLocation(white_shader, "projection"), 1, GL_FALSE, MatrixToFloatV(camera->projection).v);

	glUniform3f(glGetUniformLocation(white_shader, "cam_pos"), camera->pos.x, camera->pos.y, camera->pos.z);
	transform = MatrixTranslate(selected_pos.x, selected_pos.y, selected_pos.z);
	glUniformMatrix4fv(glGetUniformLocation(debug_chunk_shader, "transform"), 1, GL_FALSE, MatrixToFloatV(transform).v);

	glLineWidth(2.f);
	glDrawArrays(GL_LINES, 0, 2);

//	glUseProgram(debug_chunk_shader);
//	Mesh_bind(chunk_debug_mesh);
//
//	glUniformMatrix4fv(glGetUniformLocation(debug_chunk_shader, "view"), 1, GL_FALSE, MatrixToFloatV(camera->view).v);
//	glUniformMatrix4fv(glGetUniformLocation(debug_chunk_shader, "projection"), 1, GL_FALSE, MatrixToFloatV(camera->projection).v);
//	glUniform3f(glGetUniformLocation(debug_chunk_shader, "cam_pos"), camera->pos.x, camera->pos.y, camera->pos.z);
//
//	for(unsigned int i = 0; i < CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_HEIGHT; i++) {
//		transform = MatrixTranslate(chunks[i].x * CHUNK_LENGTH + (CHUNK_LENGTH / 2.f) - .5f, (CHUNK_HEIGHT / 2.f) - .5f, chunks[i].y * CHUNK_LENGTH + (CHUNK_LENGTH / 2.f) - .5f);
//
//		glUniformMatrix4fv(glGetUniformLocation(debug_chunk_shader, "transform"), 1, GL_FALSE, MatrixToFloatV(transform).v);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//	}
}

void Renderer_clear() {
	glfwPollEvents();
	glfwSwapBuffers(window);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// TODO: actually implement this
void Renderer_free() {
	glDeleteTextures(1, &texture);
	glDeleteShader(block_shader);
	glDeleteShader(debug_chunk_shader);
	glfwTerminate();
}
