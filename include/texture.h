#pragma once

#include "utils.h"

#include <glew.h>

class Texture {
public:
	void init(const char* filename);
	void use();
	void destroy();

private:
	GLuint tex;
};