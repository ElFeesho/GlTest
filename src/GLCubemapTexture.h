#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GLCubemapTexture {
public:
	GLCubemapTexture(int width, int height, unsigned char *top, unsigned char *bottom, unsigned char *left, unsigned char *right, unsigned char *front, unsigned char *back);

	void bind(int textureNumber);

private:
	GLuint _textureId;
};