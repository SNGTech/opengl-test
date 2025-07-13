#version 460 core

in vec4 outColour;

out vec4 fragColour;

void main() {
  fragColour = outColour;
}
