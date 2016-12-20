#pragma once

#include <glew.h>

class Texture {
public:
	void init(const char* filename, int width, int height);
	void use();
	void destroy();

private:
	GLuint tex;
};