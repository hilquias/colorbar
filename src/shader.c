/* ================================================================ */
/* ================================================================ */

#include <GL/glew.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "shader.h"

/* ================================================================ */
/* ================================================================ */

char *read_text_file(const char *path);

GLuint make_shader(GLenum type, const char *source);

GLuint load_shader(GLenum type, const char *path);

GLuint make_program(GLuint shader1, GLuint shader2);

/* ================================================================ */
/* ================================================================ */

void load_program(Shader *shader, const char *path1, const char *path2)
{
  GLuint shader1 = load_shader(GL_VERTEX_SHADER, path1);
  GLuint shader2 = load_shader(GL_FRAGMENT_SHADER, path2);
  GLuint program = make_program(shader1, shader2);
  shader->id = program;
}

void set_uniform_int(Shader shader, const char *name, int value)
{
  glUniform1i(glGetUniformLocation(shader.id, name), value);
}

void set_uniform_bool(Shader shader, const char *name, bool value)
{
  glUniform1i(glGetUniformLocation(shader.id, name), (int) value);
}

void set_uniform_float(Shader shader, const char *name, float value)
{
  glUniform1f(glGetUniformLocation(shader.id, name), value);
}

void set_uniform_mat4(Shader shader, const char *name, mat4 value)
{
  glUniformMatrix4fv(glGetUniformLocation(shader.id, name), 1, GL_FALSE, (float*) value);
}

void use_program(Shader shader)
{
  glUseProgram(shader.id);
}

/* ================================================================ */
/* ================================================================ */

char *read_text_file(const char *path)
{
  FILE *file = fopen(path, "rb");
  if (!file) {
	fprintf(stderr, "fopen %s failed: %d %s\n", path, errno, strerror(errno));
	exit(1);
  }
  fseek(file, 0, SEEK_END);
  int length = ftell(file);
  rewind(file);
  char *data = calloc(length + 1, sizeof(char));
  fread(data, 1, length, file);
  fclose(file);
  return data;
}

GLuint make_shader(GLenum type, const char *source)
{
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
	GLint length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	GLchar *info = calloc(length, sizeof(GLchar));
	glGetShaderInfoLog(shader, length, NULL, info);
	fprintf(stderr, "glCompileShader failed:\n%s\n", info);
	free(info);
  }
  return shader;
}

GLuint load_shader(GLenum type, const char *path)
{
  char *data = read_text_file(path);
  GLuint result = make_shader(type, data);
  free(data);
  return result;
}

GLuint make_program(GLuint shader1, GLuint shader2)
{
  GLuint program = glCreateProgram();
  glAttachShader(program, shader1);
  glAttachShader(program, shader2);
  glLinkProgram(program);
  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
	GLint length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
	GLchar *info = calloc(length, sizeof(GLchar));
	glGetProgramInfoLog(program, length, NULL, info);
	fprintf(stderr, "glLinkProgram failed: %s\n", info);
	free(info);
  }
  glDetachShader(program, shader1);
  glDetachShader(program, shader2);
  glDeleteShader(shader1);
  glDeleteShader(shader2);
  return program;
}

/* ================================================================ */
/* ================================================================ */
