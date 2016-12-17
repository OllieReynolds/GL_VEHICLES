#pragma once

#include "..\include\draw_quad.h"

void DrawQuad::init_quad(const mat4& projection_matrix) {
	shader = {
		"shaders/uniform_MP.v.glsl",
		"shaders/uniform_colour.f.glsl"
	};
	
	shader.set_uniform("uniform_colour", vec4{1.f, 1.f, 1.f, 1.f});
	shader.set_uniform("projection", projection_matrix);

	vec2 points[4] = {
		{-0.5f, -0.5f},
		{-0.5f,  0.5f},
		{0.5f,  0.5f},
		{0.5f, -0.5f}
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 4, &points, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void DrawQuad::draw_quad(const mat4& view_matrix, const mat4& projection_matrix, const vec2& position, const vec2& size) {
	shader.use();
	glBindVertexArray(vao);

	shader.set_uniform("uniform_colour", vec4{1.f, 1.f, 1.f, 1.f});
	shader.set_uniform("model", utils::gen_model_matrix(size, position));

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(0);
	shader.release();
}

void DrawQuad::destroy_quad() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	shader.destroy();
}
