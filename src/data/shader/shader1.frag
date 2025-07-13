#version 460 core

out vec4 fragColour;

uniform vec4 vertexColour;

void main() {
  fragColour = vertexColour;
}
