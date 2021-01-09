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
	Camera_set(&cam, PI / 4.f, width, height, 0.0001f, 1000.f);
}

//------------------------------------------------------------------------


Buffer load_file(const char* filename) {
	Buffer buf;

	FILE* file = fopen(filename, "r");
	fseek(file, 0L, SEEK_END);
	buf.size = ftell(file);
	rewind(file);

	buf.data = (byte*) malloc(sizeof(byte) * (buf.size + 1));
	fread(buf.data, sizeof(byte), buf.size, file);
	buf.data[buf.size] = '\0';
	buf.size += 1;

	fclose(file);
	
	return buf;
}

uint load_shader(const char* vertex_shader_file, const char* frag_shader_file) {
	Buffer buf = load_file(frag_shader_file);

	uint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, (const GLchar *const *) &buf.data, NULL);
	glCompileShader(frag_shader);

	int success = 0;
	char info_log[512];
	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
		printf("Failed to compile frag shader: \"%s\"\n", info_log);
		exit(-1);
	}

	buf.size = 0;
	free(buf.data);

	buf = load_file(vertex_shader_file);

	uint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, (const GLchar *const *) &buf.data, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glad_glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		printf("Failed to compile vertex shader: \"%s\"\n", info_log);
		exit(-1);
	}

	buf.size = 0;
	free(buf.data);

	uint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, frag_shader);
	glLinkProgram(program);
	
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(program, 512, NULL, info_log);
		printf("Failed linking shader program: \"%s\"\n", info_log);
		exit(-1);
	}
	
	glDeleteShader(vertex_shader);
	glDeleteShader(frag_shader);
	return program;
}


uint load_texture(const char* filename) {
	stbi_set_flip_vertically_on_load(1);
	int width, height, comp;
	byte* data = stbi_load(filename, &width, &height, &comp, 0);

	if(!data) {
		printf("oh shit, texture not here boyo!\n");
	}
	printf("width: %i, %i\n", width, height);

	uint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	return texture;
}


//------------------------------------------------------------------------


void handle_input(GLFWwindow* window) {
}


//------------------------------------------------------------------------


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
		 0.f,  0.f,  0.f,  0.0f, 0.0f,
		  8f,  0.f,  0.f,  1.0f, 0.0f,
		  8f,   8f,  0.f,  1.0f, 1.0f,
		  8f,   8f,  0.f,  1.0f, 1.0f,
		 0.f,   8f,  0.f,  0.0f, 1.0f,
		 0.f,  0.f,  0.f,  0.0f, 0.0f,

		 0.f,  0.f,   8f,  0.0f, 0.0f,
		  8f,  0.f,   8f,  1.0f, 0.0f,
		  8f,   8f,   8f,  1.0f, 1.0f,
		  8f,   8f,   8f,  1.0f, 1.0f,
		 0.f,   8f,   8f,  0.0f, 1.0f,
		 0.f,  0.f,   8f,  0.0f, 0.0f,

		 0.f,  8f,   8f,  1.0f, 0.0f,
		 0.f,  8f,  0.f,  1.0f, 1.0f,
		 0.f, 0.f,  0.f,  0.0f, 1.0f,
		 0.f, 0.f,  0.f,  0.0f, 1.0f,
		 0.f, 0.f,   8f,  0.0f, 0.0f,
		 0.f,  8f,   8f,  1.0f, 0.0f,

		  8f,  8f,   8f,  1.0f, 0.0f,
		  8f,  8f,  0.f,  1.0f, 1.0f,
		  8f, 0.f,  0.f,  0.0f, 1.0f,
		  8f, 0.f,  0.f,  0.0f, 1.0f,
		  8f, 0.f,   8f,  0.0f, 0.0f,
		  8f,  8f,   8f,  1.0f, 0.0f,

		 0.f, 0.f,  0.f,  0.0f, 1.0f,
		  8f, 0.f,  0.f,  1.0f, 1.0f,
		  8f, 0.f,   8f,  1.0f, 0.0f,
		  8f, 0.f,   8f,  1.0f, 0.0f,
		 0.f, 0.f,   8f,  0.0f, 0.0f,
		 0.f, 0.f,  0.f,  0.0f, 1.0f,

		 0.f, 16.f,  0.f,  0.0f, 1.0f,
		  8f, 16.f,  0.f,  1.0f, 1.0f,
		  8f, 16.f,   8f,  1.0f, 0.0f,
		  8f, 16.f,   8f,  1.0f, 0.0f,
		 0.f, 16.f,   8f,  0.0f, 0.0f,
		 0.f, 16.f,  0.f,  0.0f, 1.0f
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
	cam = Camera_create(PI / 4.f, width, height, 0.001f, 1000.f);
	
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
	Chunk_generateMesh(0);

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

		if(glfwGetKey(window, GLFW_KEY_W))
			cam.pos = Vector3Add(cam.pos, Vector3Scale(cam.front, delta * 4.f));
			
		if(glfwGetKey(window, GLFW_KEY_S))
			cam.pos = Vector3Subtract(cam.pos, Vector3Scale(cam.front, delta * 4.f));

		if(glfwGetKey(window, GLFW_KEY_A))
			cam.pos = Vector3Subtract(cam.pos, Vector3Scale(Vector3Normalize(Vector3CrossProduct(cam.front, (Vector3) { 0, 1, 0 })), delta * 4.f));

		if(glfwGetKey(window, GLFW_KEY_D))
			cam.pos = Vector3Add(cam.pos, Vector3Scale(Vector3Normalize(Vector3CrossProduct(cam.front, (Vector3) { 0, 1, 0 })), delta * 4.f));

		Camera_update(&cam, (Vector3) { 0, 1, 0 });

		transform = MatrixIdentity();
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "transform"), 1, GL_FALSE, MatrixToFloatV(transform).v);
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, MatrixToFloatV(cam.view).v);
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, MatrixToFloatV(cam.projection).v);


		glfwSwapBuffers(window);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glBindVertexArray(vao);
		glBindTexture(GL_TEXTURE_2D, texture);
		Chunks_draw();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwPollEvents();
	}

	glfwTerminate();
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteTextures(1, &texture);
	return 0;
}
