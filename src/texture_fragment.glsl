#version 330 core

uniform sampler2D boundTexture;
in vec2 oposition;
out vec4 frag_colour;

void main() {
  frag_colour = texture(boundTexture, oposition);
}