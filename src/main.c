#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RAYMATH_IMPLEMENTATION
#define RAYMATH_STANDALONE
#include <raymath.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

#include "../include/types.h" 
#include "../include/camera.h"
#include "../include/chunk.h"

#define debug_print(x) _debug_print(x, __LINE__, __FILE__);
#define this_ran() _debug_print("This ran", __LINE__, __FILE__);


//------------------------------------------------------------------------


Camera cam;


//------------------------------------------------------------------------

void _debug_print(const char* message, uint line, const char* filename) {
	printf("%s, LINE: %u, FILE: %s\n", message, line, filename);
}

void framebuffer_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	Camera_set(&cam, PI / 2.f, width, height, 0.0001f, 3000.f);
}



void handle_input(GLFWwindow* window) {
}


//------------------------------------------------------------------------


int test_amount = 0;
float test_last = 0.f;
int main() {
	// Window setup

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "testapp", NULL, NULL);
	if(window == NULL) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed starting GLAD\n");
		return -1;
	}

	glViewport(0, 0, 800, 800);
	glfwSetFramebufferSizeCallback(window, framebuffer_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	uint shader_program = load_shader("assets/vert.glsl", "assets/frag.glsl");
	uint vao;
	uint vbo;

	float verts[] = {
		 0.f,  0.f, 0.f,  0.0f, 0.0f,
		 8.f,  0.f, 0.f,  1.0f, 0.0f,
		 8.f, 16.f, 0.f,  1.0f, 1.0f,
		 8.f, 16.f, 0.f,  1.0f, 1.0f,
		 0.f, 16.f, 0.f,  0.0f, 1.0f,
		 0.f,  0.f, 0.f,  0.0f, 0.0f,

		 0.f,  0.f, 8.f,  0.0f, 0.0f,
		 8.f,  0.f, 8.f,  1.0f, 0.0f,
		 8.f, 16.f, 8.f,  1.0f, 1.0f,
		 8.f, 16.f, 8.f,  1.0f, 1.0f,
		 0.f, 16.f, 8.f,  0.0f, 1.0f,
		 0.f,  0.f, 8.f,  0.0f, 0.0f,

		 0.f, 16.f, 8.f,  1.0f, 0.0f,
		 0.f, 16.f, 0.f,  1.0f, 1.0f,
		 0.f,  0.f, 0.f,  0.0f, 1.0f,
		 0.f,  0.f, 0.f,  0.0f, 1.0f,
		 0.f,  0.f, 8.f,  0.0f, 0.0f,
		 0.f, 16.f, 8.f,  1.0f, 0.0f,

		 8.f, 16.f, 8.f,  1.0f, 0.0f,
		 8.f, 16.f, 0.f,  1.0f, 1.0f,
		 8.f,  0.f, 0.f,  0.0f, 1.0f,
		 8.f,  0.f, 0.f,  0.0f, 1.0f,
		 8.f,  0.f, 8.f,  0.0f, 0.0f,
		 8.f, 16.f, 8.f,  1.0f, 0.0f,

		 0.f,  0.f, 0.f,  0.0f, 1.0f,
		 8.f,  0.f, 0.f,  1.0f, 1.0f,
		 8.f,  0.f, 8.f,  1.0f, 0.0f,
		 8.f,  0.f, 8.f,  1.0f, 0.0f,
		 0.f,  0.f, 8.f,  0.0f, 0.0f,
		 0.f,  0.f, 0.f,  0.0f, 1.0f,

		 0.f, 16.f, 0.f,  0.0f, 1.0f,
		 8.f, 16.f, 0.f,  1.0f, 1.0f,
		 8.f, 16.f, 8.f,  1.0f, 0.0f,
		 8.f, 16.f, 8.f,  1.0f, 0.0f,
		 0.f, 16.f, 8.f,  0.0f, 0.0f,
		 0.f, 16.f, 0.f,  0.0f, 1.0f
	};
	
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	Matrix transform;
	transform = MatrixIdentity();
	cam = Camera_create(PI / 2.f, width, height, 0.001f, 3000.f);
	cam.pos = (Vector3) { CHUNKS_RADIUS / 2.f * CHUNK_LENGTH, 10.f, CHUNKS_RADIUS / 2.f * CHUNK_LENGTH };
	
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(shader_program);
	glBindVertexArray(vao);

	uint texture = load_texture("assets/rock.png");

	float angle = -1.f;
	float delta = 0;

	float prev_time = 0;
	float pos = 0;

	Vector2 mouse_pos   = { 0 };
	Vector2 mouse_prev  = { 0 };
	Vector2 mouse_delta = { 0 };

	Chunks_init();

	while(!glfwWindowShouldClose(window)) {
		delta = glfwGetTime() - prev_time;
		prev_time = glfwGetTime();

		double mouse_x, mouse_y;
		glfwGetCursorPos(window, &mouse_x, &mouse_y);

		mouse_pos.x = mouse_x;
		mouse_pos.y = mouse_y;

		mouse_delta.x = mouse_pos.x - mouse_prev.x;
		mouse_delta.y = mouse_pos.y - mouse_prev.y;
		mouse_prev = mouse_pos;

		cam.pitch -= mouse_delta.y * delta * 0.05f;
		cam.yaw += mouse_delta.x * delta * 0.05f;

		if(pos > 1.f || pos < -1.f)
			angle *= -1.f;

		pos += angle * delta * .5f;

		if(glfwGetKey(window, GLFW_KEY_LEFT_SUPER))
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if(glfwGetKey(window, GLFW_KEY_Z) && glfwGetTime() > test_last + .02f) {
			printf("this ran\n");
			test_amount += 5;
			Chunk_test(test_amount);
			test_last = glfwGetTime();
		}
		else if(glfwGetKey(window, GLFW_KEY_X) && glfwGetTime() > test_last + .02f) {
			test_amount = (test_amount - 5 >= 0) * (test_amount - 5);
			Chunk_test(test_amount);
			test_last = glfwGetTime();
		}

		float move_speed = 1.f;
		if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
			move_speed = 2.f;

		if(glfwGetKey(window, GLFW_KEY_W))
			cam.pos = Vector3Add(cam.pos, Vector3Scale(cam.front, delta * 6.f * move_speed));
			
		if(glfwGetKey(window, GLFW_KEY_S))
			cam.pos = Vector3Subtract(cam.pos, Vector3Scale(cam.front, delta * 6.f * move_speed));

		if(glfwGetKey(window, GLFW_KEY_A))
			cam.pos = Vector3Subtract(cam.pos, Vector3Scale(Vector3Normalize(Vector3CrossProduct(cam.front, (Vector3) { 0, 1, 0 })), delta * 6.f * move_speed));

		if(glfwGetKey(window, GLFW_KEY_D))
			cam.pos = Vector3Add(cam.pos, Vector3Scale(Vector3Normalize(Vector3CrossProduct(cam.front, (Vector3) { 0, 1, 0 })), delta * 6.f * move_speed));
 
		Camera_update(&cam, (Vector3) { 0, 1, 0 });

		transform = MatrixIdentity();
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "transform"), 1, GL_FALSE, MatrixToFloatV(transform).v);
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, MatrixToFloatV(cam.view).v);
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, MatrixToFloatV(cam.projection).v);
		glUniform3f(glGetUniformLocation(shader_program, "cam_pos"), cam.pos.x, cam.pos.y, cam.pos.z);

		glfwSwapBuffers(window);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glBindVertexArray(vao);
		glBindTexture(GL_TEXTURE_2D, texture);
		Chunks_draw(shader_program);
		glBindVertexArray(vao);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwPollEvents();
	}

	glfwTerminate();
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteTextures(1, &texture);
	return 0;
}
