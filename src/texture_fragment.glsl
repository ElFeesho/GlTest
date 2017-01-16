#version 330 core

uniform vec3 lightPos;
uniform sampler2D boundTexture;
in vec2 oposition;
in vec4 onormal;
in vec3 fragPos;
out vec4 frag_colour;

void main() {
	
	float ambient = 0.9f;
	vec3 norm = normalize(onormal.xyz);
	vec3 lightDir = normalize(lightPos - fragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(1.0f, 1.0f, 1.0f);

	frag_colour = vec4((ambient + diffuse), 1.0f) * texture(boundTexture, oposition);
}