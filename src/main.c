#include <stdio.h>
#include <stdlib.h>

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../include/types.h"

#define debug_print(x) _debug_print(x, __LINE__, __FILE__);
#define this_ran() _debug_print("This ran", __LINE__, __FILE__);

void _debug_print(const char* message, uint line, const char* filename) {
	printf("%s, LINE: %u, FILE: %s\n", message, line, filename);
}

void framebuffer_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

Buffer load_file(const char* filename) {
	Buffer buf;

	FILE* file = fopen(filename, "r");
	fseek(file, 0L, SEEK_END);
	buf.size = ftell(file);
	rewind(file);

	buf.data = (byte*) malloc(sizeof(byte) * buf.size);
	fread(buf.data, sizeof(byte), buf.size, file);
	fclose(file);
	
	return buf;
}

uint load_shader(const char* vertex_shader_file, const char* frag_shader_file) {
	this_ran();
	Buffer buf = load_file(frag_shader_file);

	uint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, (const GLchar *const *) buf.data, NULL);
	glCompileShader(frag_shader);

	int success;
	char info_log[512];
	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glad_glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
		printf("Failed to compile frag shader: \"%s\"\n", info_log);
		exit(-1);
	}

	buf.size = 0;
	this_ran();
	free(buf.data);

	this_ran();
	buf = load_file(vertex_shader_file);

	uint vertex_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertex_shader, 1, (const GLchar *const *) buf.data, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glad_glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		printf("Failed to compile vertex shader: \"%s\"\n", info_log);
		exit(-1);
	}

	this_ran();
	buf.size = 0;
	this_ran();
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
	
	this_ran();
	glDeleteShader(vertex_shader);
	glDeleteShader(frag_shader);
	return program;
}


int main() {
	// Window setup

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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


	uint shader_program = load_shader("assets/vert.glsl", "assets/frag.glsl");
	uint vao;
	uint eao;
	uint vbo;

	float verts[] = {
		-0.5f, -0.5f, 0.f,
		 0.5f, -0.5f, 0.f,
		-0.5f,  0.5f, 0.f,
		 0.5f,  0.5f, 0.f,
	};
	
	uint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glGenBuffers(1, &eao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eao);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	while(!glfwWindowShouldClose(window)) {
		glClearColor(0.0, 0.4, 0.3, 1);
		glClear(GL_COLOR_BUFFER_BIT);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
