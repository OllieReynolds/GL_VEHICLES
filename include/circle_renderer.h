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
	void draw_2D(const Camera& camera, const vec2& position, const vec2& size, const vec4& colour, bool filled);
	void draw_3D(const Camera& camera, const Transform& transform, const vec4& colour, bool filled);
	void draw_3D_shadow(const Camera& camera, const Transform& transform);
	void destroy();

private:
	GLuint vao;
	GLuint vbo;
	Shader shader_2D;
	Shader shader_3D;
	Shader shader_3D_shadow;
};