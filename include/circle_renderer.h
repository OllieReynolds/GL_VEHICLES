#pragma once

#include "maths.h"
#include "shader.h"
#include "utils.h"

#include <glew.h>

using namespace maths;
using namespace utils;

class Circle_Renderer {
public:
	Circle_Renderer() { }

	void init();
	void draw_2D(const mat4& view_matrix, const mat4& projection_matrix, const vec2& position, const vec2& size, const vec4& colour, bool filled);
	void draw_3D(const mat4& view_matrix, const mat4& projection_matrix, const vec3& position, const vec3& size, const vec3& rotation, const vec4& colour, bool filled);
	void destroy();

private:
	GLuint vao;
	GLuint vbo;
	Shader shader_2D;
	Shader shader_3D;
};