#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../include/types.h"


#define PI 3.14159

#define debug_print(x) _debug_print(x, __LINE__, __FILE__);
#define this_ran() _debug_print("This ran", __LINE__, __FILE__);


//------------------------------------------------------------------------

void _debug_print(const char* message, uint line, const char* filename) {
	printf("%s, LINE: %u, FILE: %s\n", message, line, filename);
}

void framebuffer_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
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



//------------------------------------------------------------------------


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
	uint ebo;
	uint vbo;

	float verts[] = {
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
	};
	
	uint indices[] = {
		// front
		1, 2, 3,
		0, 1, 3,

		// left
		1, 5, 2,
		5, 6, 2,

		// right
		4, 0, 7,
		0, 3, 7,

		// back
		5, 4, 6,
		4, 7, 6,
		
		// top
		4, 5, 0,
		5, 1, 0,

		// bottom
		3, 2, 7,
		2, 6, 7
	};

	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glGenBuffers(1, &ebo);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	float transform[16] = {
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

//	float angle = PI * 0.25f;
//	transform[5] =   cos(angle);
//	transform[6] =  -sin(angle);
//	transform[9] =   sin(angle);
//	transform[10] =  cos(angle);

	glUniformMatrix4fv(glGetUniformLocation(shader_program, "transform"), 1, GL_FALSE, transform);

	glUseProgram(shader_program);
	glBindVertexArray(vao);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while(!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glClearColor(.25, .5, .25, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
