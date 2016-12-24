#pragma once

#include "maths.h"
#include "shader.h"
#include "utils.h"

#include <glew.h>

using namespace maths;
using namespace utils;

class Line_Renderer {
public:
	Line_Renderer(const vec4& colour = utils::data::colour::black) : colour(colour) { }

	void init();
	void draw(const mat4& view_matrix, const mat4& projection_matrix, const vec3& a, const vec3& b);
	void destroy();

	vec4 colour;

private:
	GLuint vao;
	GLuint vbo;
	utils::Shader shader;
};