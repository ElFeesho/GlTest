#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texel;

out vec2 outTexel;

uniform mat4 projection;
uniform mat4 modelView;

void main() {
	gl_Position = projection * (modelView * vec4(0.0, 0.0, 0.0, 1.0) 
	+ vec4(position.x, position.y, 0.0, 0.0));
	outTexel = texel;
} 