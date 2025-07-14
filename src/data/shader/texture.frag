#version 460 core

out vec4 fragColour;

in vec3 outColour;
in vec2 outTexCoord;

uniform sampler2D textureImg;

void main(){
  fragColour = texture(textureImg, outTexCoord * 2.5f) * vec4(outColour, 1.0f);  
}
