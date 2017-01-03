#pragma once

#include "maths.h"
#include "shader.h"
#include "utils.h"

#include <glew.h>

using namespace maths;
using namespace utils;

class Line_Renderer {
public:
	Line_Renderer() { }

	void init();
	void draw(const Camera& camera, const vec3& world_space_a, const vec3& world_space_b, const vec4& colour);
	void destroy();

private:
	GLuint vao;
	GLuint vbo;
	utils::Shader shader;
};