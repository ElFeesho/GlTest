#version 330 core

in vec4 fragColour;
out vec4 frag_colour;
in vec2 oposition;

void main() {
  frag_colour = fragColour;
}