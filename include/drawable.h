#pragma once

#include <glew.h>

#include "shader.h"

class Drawable {
public:
	virtual void init() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void destroy() = 0;

private:
	GLuint gl_array_object;
	GLuint gl_buffer_object;

	utils::Shader shader;
};