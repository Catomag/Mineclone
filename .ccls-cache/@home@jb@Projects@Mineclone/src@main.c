#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RAYMATH_IMPLEMENTATION
#define RAYMATH_STANDALONE
#include <raymath.h>

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

#include "../include/util.h"
#include "../include/types.h" 
#include "../include/camera.h"
#include "../include/renderer.h"
#include "../include/chunk.h"

#define debug_print(x) _debug_print(x, __LINE__, __FILE__);
#define this_ran() _debug_print("This ran", __LINE__, __FILE__);


//------------------------------------------------------------------------

Camera cam;

//------------------------------------------------------------------------

void _debug_print(const char* message, uint line, const char* filename) {
	printf("%s, LINE: %u, FILE: %s\n", message, line, filename);
}

//------------------------------------------------------------------------


int test_amount = 0;
float test_last = 0.f;
int main() {
	// Window setup

	int width = 800;
	int height = 800;
	Renderer_init(&cam, width, width);

	cam = Camera_create(PI / 2.f, width, height, 0.001f, 3000.f);
	cam.pos = (Vector3) { CHUNKS_RADIUS / 2.f * CHUNK_LENGTH, 10.f, CHUNKS_RADIUS / 2.f * CHUNK_LENGTH };
	
	uint texture = load_texture("assets/rock.png");

	float angle = -1.f;
	float delta = 0;

	float prev_time = 0;
	float pos = 0;

	Vector2 mouse_pos   = { 0 };
	Vector2 mouse_prev  = { 0 };
	Vector2 mouse_delta = { 0 };

	Chunks_init();

	while(!Renderer_shouldClose()) {
		delta = glfwGetTime() - prev_time;
		prev_time = glfwGetTime();

		double mouse_x, mouse_y;
		get_cursor(&mouse_x, &mouse_y);

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

		if(get_key(GLFW_KEY_LEFT_SUPER))
			set_input_mode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			set_input_mode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if(get_key(GLFW_KEY_Z) && glfwGetTime() > test_last + .02f) {
			printf("this ran\n");
			test_amount += 5;
			Chunk_test(test_amount);
			test_last = glfwGetTime();
		}
		else if(get_key(GLFW_KEY_X) && glfwGetTime() > test_last + .02f) {
			test_amount = (test_amount - 5 >= 0) * (test_amount - 5);
			Chunk_test(test_amount);
			test_last = glfwGetTime();
		}

		float move_speed = 1.f;
		if(get_key(GLFW_KEY_LEFT_SHIFT))
			move_speed = 2.f;

		if(get_key(GLFW_KEY_W))
			cam.pos = Vector3Add(cam.pos, Vector3Scale(cam.front, delta * 6.f * move_speed));
			
		if(get_key(GLFW_KEY_S))
			cam.pos = Vector3Subtract(cam.pos, Vector3Scale(cam.front, delta * 6.f * move_speed));

		if(get_key(GLFW_KEY_A))
			cam.pos = Vector3Subtract(cam.pos, Vector3Scale(Vector3Normalize(Vector3CrossProduct(cam.front, (Vector3) { 0, 1, 0 })), delta * 6.f * move_speed));

		if(get_key(GLFW_KEY_D))
			cam.pos = Vector3Add(cam.pos, Vector3Scale(Vector3Normalize(Vector3CrossProduct(cam.front, (Vector3) { 0, 1, 0 })), delta * 6.f * move_speed));
 
		Camera_update(&cam, (Vector3) { 0, 1, 0 });

		Renderer_clear();

		Renderer_display();

		glfwPollEvents();
		uint test;
		cons char* var;
		ulong tacos;
	}

	Renderer_free();
	glfwTerminate();
	return 0;
}
