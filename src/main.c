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
#include "../include/debug.h"



//------------------------------------------------------------------------

Camera cam;

//------------------------------------------------------------------------


int test_amount = 0;
float test_last = 0.f;
int main() {
	// Window setup

	int width = 800;
	int height = 800;
	Renderer_init(&cam, width, height);

	cam = Camera_create(PI / 2.f, width, height, 0.001f, 3000.f);
	cam.pos = (Vector3) { CHUNKS_RADIUS / 2.f * CHUNK_LENGTH, CHUNK_HEIGHT + 10.f, CHUNKS_RADIUS / 2.f * CHUNK_LENGTH };
	
	float angle = -1.f;
	float delta = 0;

	float total_time = 0;
	float prev_time = 0;
	float pos = 0;
	int first_loop = 1;

	Vector2 mouse_pos   = { 0 };
	Vector2 mouse_prev  = { 0 };
	Vector2 mouse_delta = { 0 };

	Chunks_init();

	while(!Renderer_shouldClose()) {
		total_time = glfwGetTime();
		delta = total_time - prev_time;
		prev_time = total_time;

		double mouse_x, mouse_y;
		get_cursor(&mouse_x, &mouse_y);

		mouse_pos.x = mouse_x;
		mouse_pos.y = mouse_y;

		mouse_delta.x = mouse_pos.x - mouse_prev.x;
		mouse_delta.y = mouse_pos.y - mouse_prev.y;
		mouse_prev = mouse_pos;

		cam.pitch -= mouse_delta.y * delta * 0.035f;
		cam.yaw += mouse_delta.x * delta * 0.035f;

		if(pos > 1.f || pos < -1.f)
			angle *= -1.f;

		pos += angle * delta * .5f;

		printf("delta: %f\n", delta);
		// stop program from doing dumb stuff
		if(delta >= 2.f && !first_loop) {
			printf("stopped program because it was running too slow\n");
			break;
		}

		if(get_key(GLFW_KEY_LEFT_SUPER))
			set_input_mode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			set_input_mode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		float move_speed = 1.f;
		if(get_key(GLFW_KEY_LEFT_SHIFT))
			move_speed = 4.f;

		if(get_key(GLFW_KEY_W))
			cam.pos = Vector3Add(cam.pos, Vector3Scale(cam.front, delta * 6.f * move_speed));
			
		if(get_key(GLFW_KEY_S))
			cam.pos = Vector3Subtract(cam.pos, Vector3Scale(cam.front, delta * 6.f * move_speed));

		if(get_key(GLFW_KEY_A))
			cam.pos = Vector3Subtract(cam.pos, Vector3Scale(Vector3Normalize(Vector3CrossProduct(cam.front, (Vector3) { 0, 1, 0 })), delta * 6.f * move_speed));

		if(get_key(GLFW_KEY_D))
			cam.pos = Vector3Add(cam.pos, Vector3Scale(Vector3Normalize(Vector3CrossProduct(cam.front, (Vector3) { 0, 1, 0 })), delta * 6.f * move_speed));

		if(get_mouse_button(GLFW_MOUSE_BUTTON_LEFT)) {
			uint chunk_id;
			Vector3 block_pos;
			
			if(Chunks_getBlockFromRay((Vector3) { cam.pos.x + .5f, cam.pos.y + .5f, cam.pos.z + .5f }, cam.front, 10.f, NULL, &chunk_id, NULL, &block_pos)) {
				Chunks_setBlock(chunk_id, world_to_block(block_pos), 0);
			}
		}

//		else if(get_mouse_button(GLFW_MOUSE_BUTTON_RIGHT)) {
//			uint chunk_id;
//			Vector3 block_pos;
//			
//			if(Chunks_getBlockFromRay((Vector3) { cam.pos.x + .5f, cam.pos.y + .5f, cam.pos.z + .5f }, cam.front, 10.f, NULL, &chunk_id, NULL, &block_pos)) {
//				Chunks_setBlock(chunk_id, block_pos, 0);
//			}
//		}
 
		cam.pos.x -= (cam.pos.x > CHUNKS_RADIUS * CHUNK_LENGTH) * CHUNKS_RADIUS * CHUNK_LENGTH;
		cam.pos.x += (cam.pos.x < -0.f) * CHUNKS_RADIUS * CHUNK_LENGTH;

		cam.pos.z -= (cam.pos.z > CHUNKS_RADIUS * CHUNK_LENGTH) * CHUNKS_RADIUS * CHUNK_LENGTH;
		cam.pos.z += (cam.pos.z < -0.f) * CHUNKS_RADIUS * CHUNK_LENGTH;

		Camera_update(&cam, (Vector3) { 0, 1, 0 });

		Chunks_update();
		Renderer_clear();
		Renderer_display();
		first_loop = 0;
	}

	Chunks_free();
	Renderer_free();

	debug_log();
	debug_log_empty();
	return 0;
}
