#include "shader.hpp"
#include "glad/gl.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  std::string vertexSource_str, fragmentSource_str;
  std::ifstream vShaderFile, fShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  std::cout << "Loading vertex shader from path: " << std::filesystem::current_path().string() << "/" << vertexPath << std::endl;
  std::cout << "Loading fragment shader from path: " << std::filesystem::current_path().string() << "/" << fragmentPath << std::endl;

  // Read vertex and shader files
  // ----------------------------
  try {
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    // Read file's buffer contents into respective shader streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // Close file handles
    vShaderFile.close();
    fShaderFile.close();

    vertexSource_str = vShaderStream.str();
    fragmentSource_str = fShaderStream.str();
  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_READ\n" << e.what() << std::endl;
  }

  const char *vertexShaderSource = vertexSource_str.c_str();
  const char *fragmentShaderSource = fragmentSource_str.c_str();

  // Build and compile the shader program
  // ------------------------------------
  GLuint vertexShader, fragmentShader;
  GLint success;
  GLchar infoLog[512];

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, std::size(infoLog), NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, std::size(infoLog), NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  id = glCreateProgram();
  glAttachShader(id, vertexShader);
  glAttachShader(id, fragmentShader);
  glLinkProgram(id);
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id, std::size(infoLog), NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  // After linking both shaders, both shaders are now obselete, so we can delete them
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::use() { glUseProgram(id); }

void Shader::setUniform1b(const std::string &name, GLboolean value) {
  glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setUniform1i(const std::string &name, GLint value) {
  glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setUniform1f(const std::string &name, GLfloat value) {
  glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setUniform4f(const std::string &name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
  glUniform4f(glGetUniformLocation(id, name.c_str()), v0, v1, v2, v3);
}
