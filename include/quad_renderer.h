#pragma once

#include "maths.h"
#include "shader.h"
#include "texture.h"
#include "utils.h"

using namespace maths;
using namespace utils;

class Quad_Renderer {
public:
	Quad_Renderer(const vec4& colour = utils::data::colour::black) : colour(colour) {}

	void init(const char* texture_filename);
	void draw_2D(const mat4& view_matrix, const mat4& projection_matrix, const vec2& position, const vec2& size, const vec4& colour);
	void draw_3D(const mat4& view_matrix, const mat4& projection_matrix, const vec3& position, const vec3& size, const vec3& rotation, const vec4& colour);
	void destroy();

	vec4 colour;

private:
	GLuint vao;
	GLuint vbo;
	Shader shader_2D;
	Shader shader_3D;
	Texture texture_A;
};