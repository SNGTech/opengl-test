#include <cstddef>
#include <cstdio>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <iterator>

#define WIDTH 800
#define HEIGHT 600
#define TITLE "OpenGL Test"

void framebuffer_size_callback(GLFWwindow *, int width, int height);
void process_input(GLFWwindow *window);

// REFACTOR LATER INTO STANDALONE SHADER CLASS
const char *vertexShaderSource = "#version 460 core                                    \n"
                                 "layout (location = 0) in vec3 aPos;                  \n"
                                 "                                                     \n"
                                 "void main()                                          \n"
                                 "{                                                    \n"
                                 "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);  \n"
                                 "}                                                    \n";

const char *fragmentShaderSource = "#version 460 core                                  \n"
                                   "out vec4 FragColour;                               \n"
                                   "                                                   \n"
                                   "void main()                                        \n"
                                   "{                                                  \n"
                                   "  FragColour = vec4(0.49f, 0.67f, 0.55f, 1.0f);    \n"
                                   "}                                                  \n";

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

  printf("Loaded OpenGL version %i.%i\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

  // ------------ Initalise Shaders and Vertices [START] ------------

  // Build and compile the shader program
  // ------------------------------------
  GLuint vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, std::size(infoLog), NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  GLuint fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, std::size(infoLog), NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  GLuint shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, std::size(infoLog), NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  // After linking both shaders, both shaders are now obselete, so we can delete them
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Setup vertex data and buffers, and configure vertex attributes
  // --------------------------------------------------------------
  // clang-format off
  GLfloat vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left   
  };
  GLuint indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3, // second triangle
  };
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
  // Bind and set element buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  // Set vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);

  // Unbind vertex objects (not necessary) *unbind VAO and VBO first before unbinding EBO 
  // glBindVertexArray(0);
  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
 
  // ------------- Initalise Shaders and Vertices [END] -------------

  // Enable/disable wireframe mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // Input
    process_input(window);

    // ------------ Rendering Logic [START] ------------

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
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

void framebuffer_size_callback(GLFWwindow *, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
