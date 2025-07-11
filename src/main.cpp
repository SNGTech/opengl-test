#include <cstddef>
#include <cstdio>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define WIDTH 800
#define HEIGHT 600
#define TITLE "OpenGL Test"

void framebuffer_size_callback(GLFWwindow *, int width, int height);
void process_input(GLFWwindow *window);

int main() {
  std::cout << "Starting OpenGL Test" << std::endl;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create GLFW Window
  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window!" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Init OpenGL context using GLAD2, otherwise OpenGL function calls won't work
  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    std::cout << "Failed to initialise OpenGL context!" << std::endl;
    return -1;
  }

  printf("Loaded OpenGL version %i.%i\n", GLAD_VERSION_MAJOR(version),
         GLAD_VERSION_MINOR(version));

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // Input
    process_input(window);

    // ------------ Rendering Logic [START] ------------
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);





    // ------------- Rendering Logic [END] -------------

    glfwSwapBuffers(window); // Enable double buffering (front and back buffers)
    glfwPollEvents();
  }

  glfwTerminate();
  std::cout << "Closed OpenGL Test application" << std::endl;
  return 0;
}

void framebuffer_size_callback(GLFWwindow *, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
