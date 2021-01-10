#include <stdlib.h>
#include <stdio.h>

#include "../include/util.h"
#include "../include/glad/glad.h"
#include "../include/stb_image.h"

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
