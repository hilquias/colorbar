#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>
#include <cglm/mat4.h>

typedef struct Shader {
  GLuint id;
} Shader;

void load_program(Shader *shader, const char *path1, const char *path2);

void set_uniform_int(Shader shader, const char *name, int value);

void set_uniform_bool(Shader shader, const char *name, bool value);

void set_uniform_float(Shader shader, const char *name, float value);

void set_uniform_mat4(Shader shader, const char *name, mat4 value);

void use_program(Shader shader);

#endif
