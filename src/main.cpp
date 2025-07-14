#include "shader.hpp"
#include "stb_image.h"
#include <cmath>
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
  glfwWindowHint(GLFW_SAMPLES, 4);

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
  glEnable(GL_MULTISAMPLE);

  printf("Loaded OpenGL version %i.%i\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

  // Initialise shaders
  // ------------------
  Shader shader1("data/shader/shader1.vert", "data/shader/shader1.frag");
  Shader rainbowShader("data/shader/rainbow_v.vert", "data/shader/rainbow_v.frag");
  Shader invertedShader("data/shader/inverted_shader.vert", "data/shader/shader1.frag");
  Shader xOffsetShader("data/shader/xoffset_shader.vert", "data/shader/shader1.frag");
  Shader cVerticesShader("data/shader/cvertices.vert", "data/shader/rainbow_v.frag");
  Shader textureShader("data/shader/texture.vert", "data/shader/texture.frag");

  // Set current shader
  Shader currentShader = textureShader;

  // Setup vertex data and buffers, and configure vertex attributes
  // --------------------------------------------------------------
  // clang-format off
  GLfloat vertices[] = {
    // positions         // colours           // texture coords
     0.5f,  0.5f, 0.0f,  1.0f, 0.65f, 0.95f,    1.0f, 1.0f,         // top right
     0.5f, -0.5f, 0.0f,  0.9f, 0.9f, 0.9f,  1.0f, 0.0f,         // bottom right
    -0.5f, -0.5f, 0.0f,  0.9f, 0.8f, 1.0f,    0.0f, 0.0f,         // bottom left
    -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.8f,   0.0f, 1.0f,         // top left   
  };
  GLuint indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3, // second triangle
  };
  // float vertices[] = {
  //   // positions         // colors          // texture coords
  //    0.5f, -0.5f, 0.0f,  0.8f, 0.0f, 0.0f,  1.0f, 0.0f,        // bottom right
  //   -0.5f, -0.5f, 0.0f,  0.0f, 0.8f, 0.0f,  0.0f, 0.0f,        // bottom left
  //    0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 0.8f,  0.5f, 1.0f,        // top 
  // };  
  // clang-format on 

  GLuint VBO, VAO, EBO;
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);

  // Bind vertex array first
  glBindVertexArray(VAO);
  // Bind and set vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // // Bind and set element buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  // Set vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);
  // Set colour of each vertex using rainbow_v shaders
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  // Set texture coords of each vertex using texture shaders
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  // Unbind vertex objects (not necessary) *unbind VAO and VBO first before unbinding EBO 
  // glBindVertexArray(0);
  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
  // Setup textures
  // --------------
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Set texture wrapping and filtering methods
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Load and generate texture
  int texWidth, texHeight, nChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("assets/grass.png", &texWidth, &texHeight, &nChannels, 3);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture!" << std::endl;
  }
  stbi_image_free(data);

  // Enable/disable wireframe mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // Input
    process_input(window);

    // Render logic
    // ------------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind texture to texture shader 
    // ------------
    glBindTexture(GL_TEXTURE_2D, texture);

    currentShader.use();

    // Animate triangle colour
    // float time = glfwGetTime();
    // float triangleRGBA = std::sin(time * 2.0f) + 0.5f;
    // currentShader.setUniform4f("vertexColour", 0.0f, triangleRGBA * 0.2f, triangleRGBA, 1.0f);

    // // X offset Shader: Move vertices to the left then to the right by 20%
    // float xOffset = 0.2f * std::cos(time * 2.0f);
    // currentShader.setUniform1f("xOffset", xOffset);

    // Render triangle
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window); // Enable double buffering (front and back buffers)
    glfwPollEvents();
  }

  glfwTerminate();
  std::cout << "Closed OpenGL Test application" << std::endl;
  return 0;
}

void framebuffer_size_callback(GLFWwindow *, int width, int height) { glViewport(0, 0, width, height); }

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
