#pragma once

#include "maths.h"
#include "shader.h"
#include "global_data.h"
#include "shared.h"

#include <glew.h>

using namespace maths;
using namespace utils;

class Draw_Line {
public:
	Draw_Line(const vec4& colour = utils::colour::black) : colour(colour) { }

	void init_line(const mat4& projection_matrix);
	void draw_line(const mat4& view_matrix, const mat4& projection_matrix, const vec3& a, const vec3& b);
	void destroy_line();

	vec4 colour;

private:
	GLuint vao;
	GLuint vbo;
	utils::Shader shader;
};