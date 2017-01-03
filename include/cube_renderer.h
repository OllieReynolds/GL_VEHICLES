#pragma once

#include "maths.h"
#include "shader.h"
#include "utils.h"

#include <glew.h>

using namespace maths;
using namespace utils;

class Cube_Renderer {
public:
	Cube_Renderer() { }

	void init();
	void draw(const Camera& camera, const vec3& position, const vec3& size, float rotation, const vec4& colour);
	void draw_multiple(int n, const Camera& camera, const Transform* transforms, const vec4& colour);
	void destroy();

private:
	GLuint vao;
	GLuint vbo;
	utils::Shader shader;
};