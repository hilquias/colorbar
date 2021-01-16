/* ================================================================ */
/* ================================================================ */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <cglm/vec4.h>
#include <cglm/mat4.h>
#include <cglm/affine.h>
#include <cglm/cam.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "shader.h"

/* ================================================================ */
/* ================================================================ */

GLFWwindow *create_window();

/* ================================================================ */
/* ================================================================ */

int main(int argc, char **argv)
{

  GLFWwindow *window;

  /* ============================================================== */

  if (!glfwInit()) {
	return EXIT_FAILURE;
  }

  window = create_window();

  if (!window) {
	glfwTerminate();
	return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
	glfwTerminate();
	return EXIT_FAILURE;
  }

  /* ============================================================== */

  glEnable(GL_DEPTH_TEST);

  /* ============================================================== */

  Shader shader;

  load_program(&shader, "shaders/triangle.vert", "shaders/triangle.frag");

  /* ============================================================== */

  GLfloat vertices[] =
	{
	 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

  /* ============================================================== */

  GLuint VBO;

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLuint VAO;

  enum { attr_position, attr_texture };

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glEnableVertexAttribArray(attr_position);
  glEnableVertexAttribArray(attr_texture);
  glVertexAttribPointer(attr_position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
  glVertexAttribPointer(attr_texture, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  /* ============================================================== */

  GLuint texture1;

  glGenTextures(1, &texture1);

  int width;
  int height;
  int channels;

  unsigned char *image1 = SOIL_load_image("textures/container.jpg", &width, &height, &channels, SOIL_LOAD_RGB);

  glBindTexture(GL_TEXTURE_2D, texture1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);

  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);

  SOIL_free_image_data(image1);

  /* ============================================================== */

  GLuint texture2;

  glGenTextures(1, &texture2);

  unsigned char *image2 = SOIL_load_image("textures/awesomeface.png", &width, &height, &channels, SOIL_LOAD_RGB);

  glBindTexture(GL_TEXTURE_2D, texture2);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);

  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);

  SOIL_free_image_data(image2);

  /* ============================================================== */

  float control = 0.2;

  /* ============================================================== */

  while (!glfwWindowShouldClose(window)) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
	  glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
	  control += 0.1;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
	  control -= 0.1;
	}

	/* ============================================================ */

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	use_program(shader);
	set_uniform_int(shader, "texture1", 0);
	set_uniform_int(shader, "texture2", 1);

	mat4 model = GLM_MAT4_IDENTITY_INIT;
	glm_rotate(model, (float) glfwGetTime(), (vec3) { 1.0f, 0.0f, 0.0f });

	mat4 view = GLM_MAT4_IDENTITY_INIT;
	glm_translate(view, (vec3) { 0.0f, 0.0f, -3.0f });

	mat4 projection = GLM_MAT4_IDENTITY_INIT;
	glm_perspective(glm_rad(45.0f), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 100.0f, projection);

	set_uniform_mat4(shader, "model", model);
	set_uniform_mat4(shader, "view", view);
	set_uniform_mat4(shader, "projection", projection);

	set_uniform_float(shader, "control", control);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36); /* MAGIC NUMBER */
	glBindVertexArray(0);

	/* ============================================================ */

	glfwSwapBuffers(window);
	glfwPollEvents();

  }

  /* ============================================================== */

  glDeleteBuffers(1, &VBO);

  /* ============================================================== */

  glfwTerminate();
  return EXIT_SUCCESS;

}

/* ================================================================ */
/* ================================================================ */

GLFWwindow *create_window()
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  int window_width = WINDOW_WIDTH;
  int window_height = WINDOW_HEIGHT;

  GLFWmonitor* monitor;

  if (FULLSCREEN) {
	monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
	window_width = vidmode->width;
	window_height = vidmode->height;
  }

  return glfwCreateWindow(window_width, window_height, "OpenGL", monitor, NULL);
}

/* ================================================================ */
/* ================================================================ */
