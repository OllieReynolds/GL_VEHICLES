#pragma once

#include "..\include\quad_renderer.h"

void Quad_Renderer::init_quad(const mat4& projection_matrix) {
	shader_2D = {
		"shaders/uniform_MP.v.glsl",
		"shaders/uniform_colour.f.glsl"
	};

	shader_3D = {
		"shaders/uniform_MVP.v.glsl",
		"shaders/texture.f.glsl"
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_points_textured), &quad_points_textured, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	
	texture_A.init("C:/Users/Ollie/Desktop/compass.png", 512, 512);
}

void Quad_Renderer::draw_quad_2D(const mat4& view_matrix, const mat4& projection_matrix, const vec2& position, const vec2& size, const vec4& colour) {
	shader_2D.use();
	glBindVertexArray(vao);

	shader_2D.set_uniform("uniform_colour", colour);
	shader_2D.set_uniform("projection", projection_matrix);
	shader_2D.set_uniform("model", utils::gen_model_matrix(size, position));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	shader_2D.release();
}

void Quad_Renderer::draw_quad_3D(const mat4& view_matrix, const mat4& projection_matrix, const vec3& position, const vec3& size, const vec3& rotation, const vec4& colour) {
	shader_3D.use();
	glBindVertexArray(vao);

	texture_A.use();

	shader_3D.set_uniform("uniform_colour", colour);
	shader_3D.set_uniform("projection", projection_matrix);
	shader_3D.set_uniform("view", view_matrix);
	shader_3D.set_uniform("model", utils::gen_model_matrix(size, position, rotation));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	shader_3D.release();
}

void Quad_Renderer::destroy_quad() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	shader_2D.destroy();

	texture_A.destroy();
}
