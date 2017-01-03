#pragma once

#include "maths.h"
#include "shader.h"
#include "texture.h"
#include "utils.h"

using namespace maths;
using namespace utils;

class Quad_Renderer {
public:
	Quad_Renderer() {}

	void init();
	void draw_2D(const Camera& camera, const vec2& position, const vec2& size, const vec4& colour);
	void draw_3D(const Camera& camera, const Transform& transform, const vec4& colour);
	void draw_2D_textured(const Camera& camera, const vec2& position, const vec2& size, Texture& tex);
	void draw_3D_textured(const Camera& camera, const Transform& transform, Texture& tex);
	void destroy();

private:
	GLuint vao;
	GLuint vbo;
	Shader shader_2D;
	Shader shader_3D;
};