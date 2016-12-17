#pragma once

#include "..\include\draw_quad.h"

void DrawQuad::init_quad(const mat4& projection_matrix) {
	shader_2D = {
		"shaders/uniform_MP.v.glsl",
		"shaders/uniform_colour.f.glsl"
	};

	shader_3D = {
		"shaders/uniform_MVP.v.glsl",
		"shaders/uniform_colour.f.glsl"
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 4, &quad_points, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void DrawQuad::draw_quad_2D(const mat4& view_matrix, const mat4& projection_matrix, const vec2& position, const vec2& size, const vec4& colour) {
	shader_2D.use();
	glBindVertexArray(vao);

	shader_2D.set_uniform("uniform_colour", colour);
	shader_2D.set_uniform("projection", projection_matrix);
	shader_2D.set_uniform("model", utils::gen_model_matrix(size, position));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	shader_2D.release();
}

void DrawQuad::draw_quad_3D(const mat4& view_matrix, const mat4& projection_matrix, const vec3& position, const vec3& size, float rotation, const vec4& colour) {
	shader_3D.use();
	glBindVertexArray(vao);

	shader_3D.set_uniform("uniform_colour", colour);
	shader_3D.set_uniform("projection", projection_matrix);
	shader_3D.set_uniform("view", view_matrix);
	shader_3D.set_uniform("model", utils::gen_model_matrix(size, position, rotation));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	shader_3D.release();
}

void DrawQuad::destroy_quad() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	shader_2D.destroy();
}
