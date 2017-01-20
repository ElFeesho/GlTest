#version 330 core

uniform vec4 colour;
uniform sampler2D textureId;

in vec2 outTexel;
out vec4 fragColour;

void main() {
	fragColour = texture(textureId, outTexel) * colour;
}