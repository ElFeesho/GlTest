#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GLTexture {
public:
	GLTexture(unsigned char *data, int width, int height);
	void bind(int textureNumber);

private:
	GLuint _textureId;
};