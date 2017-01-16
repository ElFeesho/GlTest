#version 330 core

uniform sampler2D boundTexture;
in vec2 oposition;
in vec4 onormal;
out vec4 frag_colour;

void main() {
  frag_colour = texture(boundTexture, oposition);
}