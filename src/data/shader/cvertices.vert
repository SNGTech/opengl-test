#version 460 core

layout(location = 0) in vec4 aPos;

out vec4 outColour;

void main() {
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
  outColour = gl_Position;
}
