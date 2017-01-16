#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texel;

uniform vec4 colour;
out vec4 fragColour;
out vec2 oposition;
out vec4 onormal;
out vec3 fragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	gl_Position = ((projection * view) * model) * vec4(position.x, position.y, position.z, 1.0f);
	fragPos = (model * vec4(position.x, position.y, position.z, 1.0f)).xyz;
	fragColour = colour;
	oposition = texel;
	onormal = ((projection * view) * model) * vec4(normal, 1.0f);
} 