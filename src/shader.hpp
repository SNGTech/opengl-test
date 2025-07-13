#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>
#include <string>

class Shader {
public:
  // Program ID
  GLuint id;
  Shader(const char *vertexPath, const char *fragmentPath);
  void use();
  // Utility uniform var functions
  void setUniform1b(const std::string &name, GLboolean value);
  void setUniform1i(const std::string &name, GLint value);
  void setUniform1f(const std::string &name, GLfloat value);
  void setUniform4f(const std::string &name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
};

#endif
