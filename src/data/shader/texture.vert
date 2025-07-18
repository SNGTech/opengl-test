#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColour;
layout(location = 2) in vec2 aTexCoord;

out vec3 outColour;
out vec2 outTexCoord;

void main() {
  gl_Position = vec4(aPos, 1.0f);
  outColour = aColour;
  outTexCoord = aTexCoord;
}
